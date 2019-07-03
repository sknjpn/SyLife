#pragma once

class Viewer
{
	friend class ViewerManager;

	bool	m_isMouseOver = false;
	RectF	m_drawRect = RectF(Window::Size());
	Color	m_backgroundColor = Color(11, 22, 33, 128);

public:
	Viewer() = default;
	virtual ~Viewer() = default;

	// Set
	void	SetDrawRect(double x, double y, double w, double h) { m_drawRect = RectF(x, y, w, h); }

	// Get
	const RectF&	GetDrawRect() const { return m_drawRect; }
	bool	IsMouseOver() const { return m_isMouseOver; }

	virtual void	Init() {}
	virtual void	Update() {}
};

