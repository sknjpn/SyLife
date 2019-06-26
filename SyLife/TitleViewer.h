#pragma once

#include "Viewer.h"

class TitleViewer
	: public Viewer
{
	class Bubble
	{
	public:
		s3d::Vec3	m_position;
	};

	s3d::Array<Bubble>	m_bubbles;

public:
	TitleViewer()
	{
		m_drawRect = s3d::RectF(s3d::Window::Size());
	}

	void	Init() override;
	void	Update() override;
};

