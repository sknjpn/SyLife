#pragma once

#include "Viewer.h"

class AddNewModelViewer 
	: public Viewer
{
public:
	AddNewModelViewer()
	{
		m_drawRect = s3d::RectF(100, 150, 400, 300);
	}
};

