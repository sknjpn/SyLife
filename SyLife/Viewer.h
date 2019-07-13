#pragma once

class Viewer
{
	friend class ViewerManager;

	bool	m_isMouseOver = false;
	Vec2	m_drawPos = Vec2::Zero();
	RectF	m_drawRect = RectF(Scene::Size());
	Color	m_backgroundColor = Color(11, 22, 33, 128);
	unique_ptr<Transformer2D>	m_transformer;

public:
	virtual ~Viewer() = default;

	// Set
	void	SetBackgroundColor(const Color& color) { m_backgroundColor = color; }
	void	SetDrawPos(const Vec2& pos) { m_drawPos = pos; m_transformer.reset(); m_transformer = make_unique<Transformer2D>(Mat3x2::Translate(m_drawPos), true); }
	void	SetDrawPos(double x, double y) { SetDrawPos(Vec2(x, y)); }
	void	SetDrawRect(const Vec2& pos, const Vec2& size) { m_drawRect = RectF(pos, size); }
	void	SetDrawRect(const Vec2& size) { m_drawRect = RectF(size); }
	void	SetDrawRect(double x, double y, double w, double h) { m_drawRect = RectF(x, y, w, h); }
	void	SetDrawRect(double w, double h) { m_drawRect = RectF(w, h); }
	void	MoveDrawPos(double dx, double dy) { SetDrawPos(m_drawPos.movedBy(dx, dy)); }

	// Get
	const RectF&	GetDrawRect() const { return m_drawRect; }
	const Vec2&		GetDrawPos() const { return m_drawPos; }
	bool	IsMouseOver() const { return m_isMouseOver; }

	virtual void	Init() {}
	virtual void	Update() {}
};

