#pragma once

#include "Viewer.h"

#include "Module.h"

class ModulePropertyViewer :
	public Viewer
{
public:
	shared_ptr<ModuleModel>		m_model;

public:
	ModulePropertyViewer(const shared_ptr<ModuleModel>& model)
		: m_model(model)
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}

	void	Update() override;
};

