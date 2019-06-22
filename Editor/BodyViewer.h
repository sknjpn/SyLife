#pragma once

#include "Viewer.h"

#include "BodyModel.h"

class BodyViewer 
	: public Viewer
{
public:
	shared_ptr<BodyModel>	m_model;
	s3d::TextEditState		m_textEditState_name;
	s3d::TextEditState		m_textEditState_mass;

public:
	BodyViewer(const shared_ptr<BodyModel>& model)
		: m_model(model)
		, m_textEditState_name(s3d::Unicode::Widen(model->m_name))
		, m_textEditState_mass(s3d::ToString(model->m_mass))
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}

	void	Update() override;
};

