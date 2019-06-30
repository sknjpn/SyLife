#pragma once

class Viewer
{
public:
	s3d::RectF	m_drawRect;
	s3d::Color	m_backgroundColor = s3d::Color(11, 22, 33);

public:
	Viewer() = default;
	virtual ~Viewer() = default;

	virtual void	Init() {}
	virtual void	Update(bool isMouseOver) {}
};

