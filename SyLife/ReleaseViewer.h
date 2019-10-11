#pragma once

#include "Viewer.h"

class CellAsset;

class ReleaseViewer
	: public Viewer
{
public:
	ReleaseViewer() 
	{
		setPriority(3);
		setDrawRect(Scene::Rect()); 
		setBackgroundColor(Color(0, 0));
	}

	void	update() override;
};

