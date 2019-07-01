#pragma once

class Viewer
{
public:
	RectF	m_drawRect;
	ColorF	m_backgroundColor = Color(11, 22, 33);

public:
	Viewer() = default;
	virtual ~Viewer() = default;

	virtual void	Init() {}
	virtual void	Update() {}

	bool	IsMouseOver() const;
};

