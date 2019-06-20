#pragma once

#include "Viewer.h"

#include "MoleculeModel.h"

class MoleculePropertyViewer
	: public Viewer
{
	shared_ptr<MoleculeModel>	m_model;
	s3d::TextEditState		m_textEditState_name;
	s3d::TextEditState		m_textEditState_mass;

public:
	MoleculePropertyViewer(const shared_ptr<MoleculeModel>& model)
		: m_model(model)
		, m_textEditState_name(s3d::Unicode::Widen(model->m_name))
		, m_textEditState_mass(s3d::ToString(model->m_mass))
	{
		m_drawRect = s3d::RectF(0, 0, 600, 600);
	}

	void	Update() override;
};
