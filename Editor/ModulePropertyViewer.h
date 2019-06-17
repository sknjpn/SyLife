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
	{}

	void	Update() override;
};

