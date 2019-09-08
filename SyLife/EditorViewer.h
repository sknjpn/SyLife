#pragma once

#include "Viewer.h"
//#include "Model.h"
#include "AssetViewer.h"

class EditorViewer
	: public Viewer
{
	shared_ptr<Model>	m_selectedModel;

public:
	EditorViewer()
	{
		SetDrawRect(Scene::Size());
	}

	void	Init()
	{
		g_viewerManagerPtr->MakeViewer<AssetViewer>();
	}

	void	update()
	{
		
	}
};

