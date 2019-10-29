#pragma once

class Viewer
{
	friend class ViewerManager;

	bool	m_isRoot = false;
	bool	m_isDestroyed = false;
	bool	m_isMouseover = false;
	Vec2	m_drawPos = Vec2::Zero();
	RectF	m_viewerRect = RectF(Scene::Size());
	Color	m_backgroundColor = Color(11, 22, 33, 128);
	unique_ptr<Transformer2D>	m_transformer;
	shared_ptr<Viewer>			m_parentViewer;
	Array<shared_ptr<Viewer>>	m_childViewers;

	static void UpdateAllViewers();
public:
	virtual ~Viewer() = default;

	static void Run();
	static const shared_ptr<Viewer>& GetRootViewer();

	const shared_ptr<Viewer>& getParentViewer() const { return m_parentViewer; }

	template <typename T>
	shared_ptr<T>	getChildViewer() const
	{
		for (auto it = m_childViewers.begin(); it != m_childViewers.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) return dynamic_pointer_cast<T>(*it);

		throw Error(U"存在しないViewerを参照しました");
	}

	template <typename T, typename... Args>
	shared_ptr<T>	addChildViewer(Args... args) { return dynamic_pointer_cast<T>(m_assets.emplace_back(make_shared<T>(args))); }

	void	destroy();

	// 再帰的
	Array<shared_ptr<Viewer>>	getAllChildViewers() const;

	// Set
	void	setBackgroundColor(const Color& color) { m_backgroundColor = color; }
	void	setDrawPos(const Vec2& pos) { m_drawPos = pos; m_transformer.reset(); m_transformer = make_unique<Transformer2D>(Mat3x2::Translate(m_drawPos), true); }
	void	setDrawPos(double x, double y) { setDrawPos(Vec2(x, y)); }
	void	setViewerSize(const Vec2& size) { m_viewerRect.size = size; }
	void	setViewerSize(double x, double y) { setViewerSize(Vec2(x, y)); }
	void	setViewerPos(const Vec2& pos) { m_viewerRect.pos = pos; }
	void	setViewerPos(double x, double y) { setViewerPos(Vec2(x, y)); }
	void	setViewerRect(const Vec2& pos, const Vec2& size) { m_viewerRect = RectF(pos, size); }
	void	setViewerRect(const Rect& rect) { m_viewerRect = rect; }
	void	setViewerRect(const RectF& rect) { m_viewerRect = rect; }
	void	setViewerRect(const Vec2& size) { m_viewerRect = RectF(size); }
	void	setViewerRect(double x, double y, double w, double h) { m_viewerRect = RectF(x, y, w, h); }
	void	setViewerRect(double w, double h) { m_viewerRect = RectF(w, h); }
	void	moveDrawPos(double dx, double dy) { setDrawPos(m_drawPos.movedBy(dx, dy)); }

	// Get
	bool	isMouseover() const { return m_isMouseover; }
	const RectF& getViewerRect() const { return m_viewerRect; }
	const Vec2& getViewerSize() const { return m_viewerRect.size; }
	const Vec2		getDrawCenter() const { return m_viewerRect.center(); }
	const Vec2& getDrawPos() const { return m_drawPos; }

	virtual void	update() {}
};

