#pragma once

#include "Viewer.h"

#include "ModuleModel.h"

class ModulePropertyViewer :
	public Viewer
{
public:
	shared_ptr<ModuleModel>	m_model;
	s3d::TextEditState		m_textEditState_name;
	s3d::TextEditState		m_textEditState_mass;

public:
	ModulePropertyViewer(const shared_ptr<ModuleModel>& model)
		: m_model(model)
		, m_textEditState_name(s3d::Unicode::Widen(model->m_name))
		, m_textEditState_mass(s3d::ToString(model->m_mass))
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}

	void	Update() override;
};

