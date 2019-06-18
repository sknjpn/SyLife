#pragma once

#include "Viewer.h"
#include "ShapeModelModel.h"
#include "TinyCamera.h"
#include "Part.h"

class PartShapeModelViewer :
	public Viewer
{
public:
	shared_ptr<PartModel>	m_model;

	s3d::Array<s3d::Vec2>	m_verticles;
	
	s3d::Vec2*		m_selectedVerticle = nullptr;

	CursorCamera2D	m_camera;

public:
	PartShapeModelViewer(const shared_ptr<PartModel>& model)
		: m_model(model)
	{
		m_drawRect = s3d::RectF(0, 0, 600, 450);
	}

	void	Init() override;
	void	Update() override;
};

