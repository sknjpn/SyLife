#pragma once

#include "Viewer.h"
#include "Model.h"

class AssetViewer
	: public Viewer
{
	shared_ptr<Model>	m_selectedModel;

public:
	void	Update() override;
};

