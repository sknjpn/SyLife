#pragma once

#include "Viewer.h"

class AddModelViewer
	: public Viewer
{
public:
	AddModelViewer()
	{
		m_drawRect = s3d::RectF(600, 0, 200, 600);
	}

	void	Update() override;
};

