#pragma once

class Viewer
{
	friend class ViewerManager;

	bool	m_isMouseOver = false;
	int		m_priority = 0;
	Vec2	m_drawPos = Vec2::Zero();
	RectF	m_drawRect = RectF(Scene::Size());
	Color	m_backgroundColor = Color(11, 22, 33, 128);
	unique_ptr<Transformer2D>	m_transformer;

public:
	virtual ~Viewer() = default;

	// Set
	void	setPriority(int priority) { m_priority = priority; }
	void	setBackgroundColor(const Color& color) { m_backgroundColor = color; }
	void	setDrawPos(const Vec2& pos) { m_drawPos = pos; m_transformer.reset(); m_transformer = make_unique<Transformer2D>(Mat3x2::Translate(m_drawPos), true); }
	void	setDrawPos(double x, double y) { setDrawPos(Vec2(x, y)); }
	void	setDrawSize(const Vec2& size) { m_drawRect.size = size; }
	void	setDrawSize(double x, double y) { setDrawSize(Vec2(x, y)); }
	void	setDrawRect(const Vec2& pos, const Vec2& size) { m_drawRect = RectF(pos, size); }
	void	setDrawRect(const Rect& rect) { m_drawRect = rect; }
	void	setDrawRect(const RectF& rect) { m_drawRect = rect; }
	void	setDrawRect(const Vec2& size) { m_drawRect = RectF(size); }
	void	setDrawRect(double x, double y, double w, double h) { m_drawRect = RectF(x, y, w, h); }
	void	setDrawRect(double w, double h) { m_drawRect = RectF(w, h); }
	void	moveDrawPos(double dx, double dy) { setDrawPos(m_drawPos.movedBy(dx, dy)); }

	// Get
	const RectF&	getDrawRect() const { return m_drawRect; }
	const Vec2&		getDrawSize() const { return m_drawRect.size; }
	const Vec2		getDrawCenter() const { return m_drawRect.center(); }
	const Vec2&		getDrawPos() const { return m_drawPos; }
	bool	isMouseOver() const { return m_isMouseOver; }

	virtual void	init() {}
	virtual void	update() {}
};

