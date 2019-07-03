#pragma once

#include "Viewer.h"
#include "AssetViewer.h"

class EditorViewer
	: public Viewer
{
public:
	EditorViewer()
	{
		SetDrawRect(Window::Size());
	}

	void	Init()
	{
		g_viewerManagerPtr->MakeViewer<AssetViewer>();
	}
};

