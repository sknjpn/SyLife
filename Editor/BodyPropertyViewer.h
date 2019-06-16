#pragma once

#include "Viewer.h"

#include "Body.h"
#include "TextBox.h"

class BodyPropertyViewer :
	public Viewer
{
public:
	shared_ptr<BodyModel>	m_model;
	TextBox	m_textBox;

public:
	BodyPropertyViewer()
		: m_textBox(s3d::RectF(10, 10, 120, 20))
	{}

	void	Update() override;
};

