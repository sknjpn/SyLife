#pragma once

#include "Viewer.h"

class TitleViewer
	: public Viewer
{
	class Bubble
	{
	public:
		s3d::Vec3	m_position;
		double		m_timer = 0.0;
	};

	s3d::Array<Bubble>	m_bubbles;

public:
	TitleViewer()
	{
		m_drawRect = s3d::RectF(s3d::Window::Size());
	}

	void	UpdateBubbles();
	void	DrawBubbles();

	void	Init() override;
	void	Update() override;
};

