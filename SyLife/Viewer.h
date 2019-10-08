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
	void	SetdrawPos(const Vec2& pos) { m_drawPos = pos; m_transformer.reset(); m_transformer = make_unique<Transformer2D>(Mat3x2::Translate(m_drawPos), true); }
	void	SetdrawPos(double x, double y) { SetdrawPos(Vec2(x, y)); }
	void	SetdrawRect(const Vec2& pos, const Vec2& size) { m_drawRect = RectF(pos, size); }
	void	SetdrawRect(const Vec2& size) { m_drawRect = RectF(size); }
	void	SetdrawRect(double x, double y, double w, double h) { m_drawRect = RectF(x, y, w, h); }
	void	SetdrawRect(double w, double h) { m_drawRect = RectF(w, h); }
	void	MovedrawPos(double dx, double dy) { SetdrawPos(m_drawPos.movedBy(dx, dy)); }

	// Get
	const RectF&	GetdrawRect() const { return m_drawRect; }
	const Vec2&		GetdrawPos() const { return m_drawPos; }
	bool	IsMouseOver() const { return m_isMouseOver; }

	virtual void	Init() {}
	virtual void	update() {}
};

