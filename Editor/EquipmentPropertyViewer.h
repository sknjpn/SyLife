#pragma once

#include "Viewer.h"

#include "Equipment.h"

class EquipmentPropertyViewer :
	public Viewer
{
public:
	shared_ptr<EquipmentModel>	m_model;

public:
	EquipmentPropertyViewer(const shared_ptr<EquipmentModel>& model)
		: m_model(model)
	{}

public:
	void	Update() override;
};

