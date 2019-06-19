#pragma once

#include "Viewer.h"

#include "CellModel.h"
#include "TextBox.h"
#include "NumberBox.h"

class CellPropertyViewer
	: public Viewer
{
	shared_ptr<CellModel>	m_model;
	TextBox	m_textBox;

public:
	CellPropertyViewer(const shared_ptr<CellModel>& model)
		: m_model(model)
		, m_textBox(s3d::RectF(10, 10, 120, 20))
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}
};

