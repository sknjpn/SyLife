#pragma once

#include "Viewer.h"
#include "Model.h"

class AssetViewer
	: public Viewer
{
	shared_ptr<Model>	m_selectedModel;

public:
	AssetViewer()
	{
		m_drawRect = s3d::RectF(600, 0, 200, 600);
	}

	void	Update() override;
	void	SetSelectedModel(const shared_ptr<Model>& model);
};

