#pragma once

class Viewer
{
public:
	s3d::RectF	m_drawRect;

public:
	Viewer() = default;
	virtual ~Viewer() = default;

	virtual void	Init() {}
	virtual void	Update(bool	isMouseOver) {}
};

