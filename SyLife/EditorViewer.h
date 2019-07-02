#pragma once

#include "Viewer.h"
#include "ViewerManager.h"
#include "AssetViewer.h"

class EditorViewer
	: public Viewer
{
public:
	EditorViewer()
	{
		m_drawRect = RectF(Window::Size());
	}

	void	Init()
	{
		g_viewerManagerPtr->MakeViewer<AssetViewer>();
	}
};

