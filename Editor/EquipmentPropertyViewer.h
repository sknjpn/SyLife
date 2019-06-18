#pragma once

#include "Viewer.h"

#include "EquipmentModel.h"

class EquipmentPropertyViewer :
	public Viewer
{
public:
	shared_ptr<EquipmentModel>	m_model;

public:
	EquipmentPropertyViewer(const shared_ptr<EquipmentModel>& model)
		: m_model(model)
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}

public:
	void	Update() override;
};

