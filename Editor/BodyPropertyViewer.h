#pragma once

#include "Viewer.h"

#include "BodyModel.h"
#include "TextBox.h"

class BodyPropertyViewer :
	public Viewer
{
public:
	shared_ptr<BodyModel>	m_model;
	TextBox	m_textBox;

public:
	BodyPropertyViewer(const shared_ptr<BodyModel>& model)
		: m_model(model)
		, m_textBox(s3d::RectF(10, 10, 120, 20))
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}

	void	Update() override;
};

