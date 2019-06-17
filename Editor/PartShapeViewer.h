#pragma once

#include "Viewer.h"
#include "Shape.h"
#include "TinyCamera.h"
#include "Part.h"

class PartShapeViewer :
	public Viewer
{
public:
	shared_ptr<PartModel>	m_model;

	s3d::Array<Shape>		m_shapes;
	s3d::Array<s3d::Vec2>	m_verticles;
	
	s3d::Vec2*		m_selectedVerticle = nullptr;

	CursorCamera2D	m_camera;const shared_ptr

public:
	PartShapeViewer(const shared_ptr<PartModel>& model)
		: m_model(model)
	{}

	void	Init() override;
	void	Update() override;
};

