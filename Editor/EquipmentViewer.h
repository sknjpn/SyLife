#pragma once

#include "Viewer.h"

#include "EquipmentModel.h"

class EquipmentViewer
	: public Viewer
{
public:
	shared_ptr<EquipmentModel>	m_model;
	s3d::TextEditState		m_textEditState_name;
	s3d::TextEditState		m_textEditState_mass;

public:
	EquipmentViewer(const shared_ptr<EquipmentModel>& model)
		: m_model(model)
		, m_textEditState_name(s3d::Unicode::Widen(model->m_name))
		, m_textEditState_mass(s3d::ToString(model->m_mass))
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}

public:
	void	Update() override;
};

