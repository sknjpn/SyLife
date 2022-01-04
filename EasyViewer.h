#pragma once

#include "common.h"

class EasyViewer : public std::enable_shared_from_this<EasyViewer> {
  bool m_isMouseover = false;
  bool m_isPenetrated = false; // 下のものを当たり判定をするかどうか
  bool m_isDestroyed = false;
  bool m_moveToFrontReserved = false;
  Vec2 m_drawPos = Vec2::Zero();
  RectF m_viewerRectInLocal = RectF(Scene::Size());
  Color m_backgroundColor = Color(0, 0);
  String m_name;
  std::unique_ptr<Transformer2D> m_transformer;
  std::weak_ptr<EasyViewer> m_parentViewer;
  Array<std::shared_ptr<EasyViewer>> m_childViewers;

  void process() {
    if (m_isDestroyed)
      return;

    // 自身の更新
    if (!isRoot()) {
      std::unique_ptr<ScopedViewport2D> sv;
	  std::unique_ptr<Transformer2D> t;

      if (getViewport()) {
        const auto delta = getViewerPosInWorld() - getViewport().value().pos;

        sv = getViewport() ? std::make_unique<ScopedViewport2D>(getViewport())
                           : std::make_unique<ScopedViewport2D>(Rect());
        t = std::make_unique<Transformer2D>(Mat3x2::Translate(delta),
                          Mat3x2::Translate(getViewerPosInWorld() + delta));
      } else
        sv = std::make_unique<ScopedViewport2D>(Rect());

      RectF(m_viewerRectInLocal.size).draw(m_backgroundColor);

      update();

      m_transformer.reset();
      m_drawPos = Vec2::Zero();

      // フレーム描画
      if (KeyL.pressed())
        RectF(m_viewerRectInLocal.size)
            .drawFrame(2.0, 0.0, isMouseover() ? Palette::Red : Palette::Green);
    }

    // Childの更新
    // 長さが変わる可能性があるのでintで管理
    for (int i = 0; i < m_childViewers.size(); ++i)
      m_childViewers[i]->process();

    const auto childs = m_childViewers.filter(
        [](const auto &viewer) { return viewer->m_moveToFrontReserved; });
    for (const auto &child : childs) {
      m_childViewers.remove(child);
      m_childViewers.emplace_back(child);
      child->m_moveToFrontReserved = false;
    }
  }

  void removeDeadViewer() {
    for (const auto &cv : m_childViewers)
      cv->removeDeadViewer();

    m_childViewers.remove_if([](const auto &cv) { return cv->m_isDestroyed; });

    if (m_isDestroyed)
      onDestroy();
  }

public:
  EasyViewer() = default;
  ~EasyViewer() = default;
  EasyViewer(const EasyViewer &) = delete;
  EasyViewer &operator=(const EasyViewer &) = delete;
  EasyViewer(EasyViewer &&) = delete;
  EasyViewer &operator=(EasyViewer &&) = delete;

  static std::shared_ptr<EasyViewer> &GetRootViewer() {
    static std::shared_ptr<EasyViewer> rootViewer =
        std::make_shared<EasyViewer>();

    return rootViewer;
  }

  static void Run() {
    while (System::Update()) {
      // ログ出力のクリア
      ClearPrint();

      // MouseOverの計算
      {
        const auto viewers = GetRootViewer()->getAllChildViewers();

        for (auto it = viewers.begin(); it < viewers.end(); ++it)
          (*it)->m_isMouseover =
              (*it)->getViewport() && (*it)->getViewport().value().mouseOver();

        for (auto it = viewers.rbegin(); it != viewers.rend(); ++it) {
          if ((*it)->m_isMouseover && !(*it)->m_isPenetrated) {
            ++it;

            for (; it != viewers.rend(); ++it)
              (*it)->m_isMouseover = false;

            break;
          }
        }
      }

      // EasyViewer 更新
      GetRootViewer()->process();

      // destroyされたEasyViewerの削除
      GetRootViewer()->removeDeadViewer();
    }
  }

  std::shared_ptr<EasyViewer> getParentViewer() const {
    return m_parentViewer.lock();
  }

  template <typename T> std::shared_ptr<T> getParentViewer() const {
    if (std::dynamic_pointer_cast<T>(m_parentViewer.lock()) != nullptr)
      return std::dynamic_pointer_cast<T>(m_parentViewer.lock());

    return nullptr;
  }

  template <typename T> std::shared_ptr<T> getChildViewer() const {
    for (auto it = m_childViewers.begin(); it != m_childViewers.end(); ++it)
      if (std::dynamic_pointer_cast<T>(*it) != nullptr)
        return std::dynamic_pointer_cast<T>(*it);

    return nullptr;
  }

  template <typename T>
  std::shared_ptr<T> getChildViewer(const String &name) const {
    for (auto it = m_childViewers.begin(); it != m_childViewers.end(); ++it)
      if (std::dynamic_pointer_cast<T>(*it) != nullptr && (*it)->m_name == name)
        return std::dynamic_pointer_cast<T>(*it);

    return nullptr;
  }

  template <typename T> bool hasChildViewer() const {
    for (auto it = m_childViewers.begin(); it != m_childViewers.end(); ++it)
      if (std::dynamic_pointer_cast<T>(*it) != nullptr)
        return true;

    return false;
  }

  bool hasChildViewer(const String &name) const {
    for (auto it = m_childViewers.begin(); it != m_childViewers.end(); ++it)
      if ((*it)->getName() == name)
        return true;

    return false;
  }

  template <typename T, typename... Args>
  std::shared_ptr<T> addChildViewer(Args &&...args) {
    auto cv = std::make_shared<T>(args...);

    m_childViewers.emplace_back(cv);
    cv->m_parentViewer = shared_from_this();
    cv->m_viewerRectInLocal.size = m_viewerRectInLocal.size;
    cv->init();

    return cv;
  }

  void destroy() {
    if (isRoot())
      return;

    m_isDestroyed = true;

    for (auto &cv : m_childViewers)
      cv->destroy();
  }

  const Array<std::shared_ptr<EasyViewer>> &getChildViewers() const {
    return m_childViewers;
  }

  template <typename T> Array<std::shared_ptr<T>> getChildViewers() const {
    Array<std::shared_ptr<T>> tChildViewers;

    for (auto it = m_childViewers.begin(); it != m_childViewers.end(); ++it)
      if (std::dynamic_pointer_cast<T>(*it) != nullptr)
        tChildViewers.emplace_back(std::dynamic_pointer_cast<T>(*it));

    return tChildViewers;
  }

  // 再帰的に取得する
  Array<std::shared_ptr<EasyViewer>> getAllChildViewers() const {
    Array<std::shared_ptr<EasyViewer>> result;

    result.append(m_childViewers);

    for (const auto &cv : m_childViewers)
      result.append(cv->getAllChildViewers());

    return result;
  }

  // Set
  std::shared_ptr<EasyViewer> setBackgroundColor(const Color &color) {
    m_backgroundColor = color;
    return shared_from_this();
  }
  std::shared_ptr<EasyViewer> setDrawPos(const Vec2 &pos) {
    m_drawPos = pos;
    m_transformer.reset();
    m_transformer =
        std::make_unique<Transformer2D>(Mat3x2::Translate(m_drawPos), true);
    return shared_from_this();
  }
  std::shared_ptr<EasyViewer> setDrawPos(double x, double y) {
    setDrawPos(Vec2(x, y));
    return shared_from_this();
  }
  std::shared_ptr<EasyViewer> setViewerSize(const Vec2 &size) {
    m_viewerRectInLocal.size = size;
    return shared_from_this();
  }
  std::shared_ptr<EasyViewer> setViewerSize(double x, double y) {
    setViewerSize(Vec2(x, y));
    return shared_from_this();
  }
  std::shared_ptr<EasyViewer> setViewerPosInLocal(const Vec2 &pos) {
    m_viewerRectInLocal.pos = pos;
    return shared_from_this();
  }
  std::shared_ptr<EasyViewer> setViewerPosInLocal(double x, double y) {
    setViewerPosInLocal(Vec2(x, y));
    return shared_from_this();
  }
  std::shared_ptr<EasyViewer> setViewerRectInLocal(const Vec2 &pos,
                                                   const Vec2 &size) {
    m_viewerRectInLocal = RectF(pos, size);
    return shared_from_this();
  }
  std::shared_ptr<EasyViewer> setViewerRectInLocal(const Rect &rect) {
    m_viewerRectInLocal = rect;
    return shared_from_this();
  }
  std::shared_ptr<EasyViewer> setViewerRectInLocal(const RectF &rect) {
    m_viewerRectInLocal = rect;
    return shared_from_this();
  }
  std::shared_ptr<EasyViewer> setViewerRectInLocal(const Vec2 &size) {
    m_viewerRectInLocal = RectF(size);
    return shared_from_this();
  }
  std::shared_ptr<EasyViewer> setViewerRectInLocal(double x, double y, double w,
                                                   double h) {
    m_viewerRectInLocal = RectF(x, y, w, h);
    return shared_from_this();
  }
  std::shared_ptr<EasyViewer> setViewerRectInLocal(double w, double h) {
    m_viewerRectInLocal = RectF(w, h);
    return shared_from_this();
  }
  std::shared_ptr<EasyViewer> moveDrawPos(double dx, double dy) {
    setDrawPos(m_drawPos.movedBy(dx, dy));
    return shared_from_this();
  }
  std::shared_ptr<EasyViewer> setName(const String &name) {
    m_name = name;
    return shared_from_this();
  }
  std::shared_ptr<EasyViewer> setIsPenetrated(bool isPenetrated) {
    m_isPenetrated = isPenetrated;
    return shared_from_this();
  }

  Optional<Rect> getViewport() const {
    if (isRoot())
      return Scene::Rect();

    auto tRect = Rect(getViewerRectInWorld());

    if (!getParentViewer()->getViewport())
      return none;

    auto pRect = getParentViewer()->getViewport().value();

    if (pRect.contains(tRect))
      return tRect;
    if (!pRect.intersects(tRect))
      return none;

    if (tRect.tl().x < pRect.tl().x) {
      tRect.size.x -= (pRect.tl().x - tRect.tl().x);
      tRect.pos.x += (pRect.tl().x - tRect.tl().x);
    }

    if (tRect.tl().y < pRect.tl().y) {
      tRect.size.y -= (pRect.tl().y - tRect.tl().y);
      tRect.pos.y += (pRect.tl().y - tRect.tl().y);
    }

    if (pRect.br().x < tRect.br().x)
      tRect.size.x -= tRect.br().x - pRect.br().x;

    if (pRect.br().y < tRect.br().y)
      tRect.size.y -= tRect.br().y - pRect.br().y;

    return tRect;
  }

  bool isMouseover() const { return m_isMouseover; }
  bool isRoot() const { return GetRootViewer() == shared_from_this(); }
  RectF getViewerRectInWorld() const {
    return isRoot() ? RectF(Scene::Rect())
                    : RectF(getViewerPosInWorld(), m_viewerRectInLocal.size);
  }
  Vec2 getViewerPosInWorld() const {
    if (isRoot())
      return Vec2::Zero();
    else
      return m_viewerRectInLocal.pos.movedBy(
          getParentViewer()->getViewerPosInWorld());
  }
  const RectF &getViewerRectInLocal() const { return m_viewerRectInLocal; }
  const Vec2 &getViewerPosInLocal() const { return m_viewerRectInLocal.pos; }
  const Vec2 &getViewerSize() const { return m_viewerRectInLocal.size; }
  const Vec2 &getDrawPos() const { return m_drawPos; }
  const String &getName() const { return m_name; }

  void moveToFront() { m_moveToFrontReserved = true; }

  virtual void init() {}
  virtual void update() {}
  virtual void onDestroy() {}
};
