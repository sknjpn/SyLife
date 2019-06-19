#pragma once

#include "Viewer.h"

#include "CellModel.h"

class CellPropertyViewer
	: public Viewer
{
	shared_ptr<CellModel>	m_model;
	s3d::TextEditState		m_textEditState_name;

public:
	CellPropertyViewer(const shared_ptr<CellModel>& model)
		: m_model(model)
		, m_textEditState_name(s3d::Unicode::Widen(model->m_name))\
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}

	void	Update() override;
};

