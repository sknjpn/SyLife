#pragma once

#include "stdafx.h"

class Viewer
	: public enable_shared_from_this<Viewer>
{
	friend class ViewerManager;

	bool	m_isRoot = false;
	bool	m_isDestroyed = false;
	Vec2	m_drawPos = Vec2::Zero();
	RectF	m_viewerRectInLocal = RectF(Scene::Size());
	Color	m_backgroundColor = Color(0, 0);
	String	m_name;
	unique_ptr<Transformer2D>	m_transformer;
	shared_ptr<Viewer>			m_parentViewer;
	Array<shared_ptr<Viewer>>	m_childViewers;
	static shared_ptr<Viewer>	g_mouseoveredViewer;

	static void UpdateAllViewers();
	void	process();
	void	removeDeadViewer();

public:
	virtual ~Viewer() = default;

	static void Run();
	static const shared_ptr<Viewer>& GetRootViewer();

	const shared_ptr<Viewer>& getParentViewer() const { return m_parentViewer; }

	template <typename T>
	shared_ptr<T>	getParentViewer() const
	{
		if (dynamic_pointer_cast<T>(m_parentViewer) != nullptr) return dynamic_pointer_cast<T>(m_parentViewer);

		return nullptr;
	}

	template <typename T>
	shared_ptr<T>	getChildViewer() const
	{
		for (auto it = m_childViewers.begin(); it != m_childViewers.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) return dynamic_pointer_cast<T>(*it);

		return nullptr;
	}

	template <typename T>
	shared_ptr<T>	getChildViewer(const String& name) const
	{
		for (auto it = m_childViewers.begin(); it != m_childViewers.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr && (*it)->m_name == name) return dynamic_pointer_cast<T>(*it);

		return nullptr;
	}

	template <typename T>
	bool	hasChildViewer() const
	{
		for (auto it = m_childViewers.begin(); it != m_childViewers.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) return true;

		return false;
	}

	template <typename T, typename... Args>
	shared_ptr<T>	addChildViewer(Args&&... args)
	{
		auto cv = make_shared<T>(args...);

		m_childViewers.emplace_back(cv);
		cv->m_parentViewer = shared_from_this();
		cv->init();

		return cv;
	}

	void	destroy();

	const Array<shared_ptr<Viewer>>& getChildViewers() const { return m_childViewers; }

	template <typename T>
	Array<shared_ptr<T>>	getChildViewers() const
	{
		Array<shared_ptr<T>> tChildViewers;

		for (auto it = m_childViewers.begin(); it != m_childViewers.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) tChildViewers.emplace_back(dynamic_pointer_cast<T>(*it));

		return tChildViewers;
	}

	// 再帰的
	Array<shared_ptr<Viewer>>	getAllChildViewers() const;

	// Set
	void	setBackgroundColor(const Color& color) { m_backgroundColor = color; }
	void	setDrawPos(const Vec2& pos) { m_drawPos = pos; m_transformer.reset(); m_transformer = MakeUnique<Transformer2D>(Mat3x2::Translate(m_drawPos), true); }
	void	setDrawPos(double x, double y) { setDrawPos(Vec2(x, y)); }
	void	setViewerSize(const Vec2& size) { m_viewerRectInLocal.size = size; }
	void	setViewerSize(double x, double y) { setViewerSize(Vec2(x, y)); }
	void	setViewerPosInLocal(const Vec2& pos) { m_viewerRectInLocal.pos = pos; }
	void	setViewerPosInLocal(double x, double y) { setViewerPosInLocal(Vec2(x, y)); }
	void	setViewerRectInLocal(const Vec2& pos, const Vec2& size) { m_viewerRectInLocal = RectF(pos, size); }
	void	setViewerRectInLocal(const Rect& rect) { m_viewerRectInLocal = rect; }
	void	setViewerRectInLocal(const RectF& rect) { m_viewerRectInLocal = rect; }
	void	setViewerRectInLocal(const Vec2& size) { m_viewerRectInLocal = RectF(size); }
	void	setViewerRectInLocal(double x, double y, double w, double h) { m_viewerRectInLocal = RectF(x, y, w, h); }
	void	setViewerRectInLocal(double w, double h) { m_viewerRectInLocal = RectF(w, h); }
	void	moveDrawPos(double dx, double dy) { setDrawPos(m_drawPos.movedBy(dx, dy)); }

	shared_ptr<Viewer>	setName(const String& name) { m_name = name; return shared_from_this(); }

	Optional<Rect>	getViewport() const;

	// Get
	bool	isMouseover() const;
	RectF	getViewerRectInWorld() const { return m_isRoot ? RectF(Scene::Rect()) : RectF(getViewerPosInWorld(), m_viewerRectInLocal.size); }
	Vec2	getViewerPosInWorld() const;
	const RectF& getViewerRectInLocal() const { return m_viewerRectInLocal; }
	const Vec2& getViewerPosInLocal() const { return m_viewerRectInLocal.pos; }
	const Vec2& getViewerSize() const { return m_viewerRectInLocal.size; }
	const Vec2& getDrawPos() const { return m_drawPos; }
	const String& getName() const { return m_name; }

	virtual void	init() {}
	virtual void	update() {}
};
