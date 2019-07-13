#pragma once

#include "Viewer.h"
#include "AssetViewer.h"

class EditorViewer
	: public Viewer
{
public:
	EditorViewer()
	{
		SetDrawRect(Scene::Size());
	}

	void	Init()
	{
		g_viewerManagerPtr->MakeViewer<AssetViewer>();
	}
};

