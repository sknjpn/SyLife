#pragma once

#include "Viewer.h"

class TitleViewer
	: public Viewer
{
public:
	TitleViewer()
	{
		m_drawRect = s3d::RectF(s3d::Window::Size());
	}

	void	Init() override;
	void	Update() override;
};

