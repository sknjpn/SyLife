#pragma once

class Viewer
{
	friend class ViewerManager;

	bool	m_isMouseOver = false;
	RectF	m_drawRect = RectF(Window::Size());
	Color	m_backgroundColor = Color(11, 22, 33, 128);

public:
	virtual ~Viewer() = default;

	// Set
	void	SetDrawRect(double x, double y, double w, double h) { m_drawRect = RectF(x, y, w, h); }
	void	SetDrawRect(double w, double h) { m_drawRect = RectF(w, h); }
	void	SetDrawRect(const Vec2& pos, const Vec2& size) { m_drawRect = RectF(pos, size); }
	void	SetDrawRect(const Vec2& size) { m_drawRect = RectF(size); }

	// Get
	const RectF&	GetDrawRect() const { return m_drawRect; }
	bool	IsMouseOver() const { return m_isMouseOver; }

	virtual void	Init() {}
	virtual void	Update() {}
};

