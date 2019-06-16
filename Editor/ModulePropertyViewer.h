#pragma once

#include "Viewer.h"

#include "Module.h"

class ModulePropertyViewer :
	public Viewer
{
public:
	shared_ptr<ModuleModel>		m_model;

public:
	void	Update() override;
};

