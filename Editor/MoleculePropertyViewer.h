#pragma once

#include "Viewer.h"

#include "MoleculeModel.h"
#include "TextBox.h"
#include "NumberBox.h"

class MoleculePropertyViewer
	: public Viewer
{
	shared_ptr<MoleculeModel>	m_model;
	TextBox	m_textBox;

public:
	MoleculePropertyViewer(const shared_ptr<MoleculeModel>& model)
		: m_model(model)
		, m_textBox(s3d::RectF(10, 10, 120, 20))
	{
		m_drawRect = s3d::RectF(0, 0, 600, 600);
	}
};

