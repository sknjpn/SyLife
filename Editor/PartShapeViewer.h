#pragma once

#include "Viewer.h"
#include "ShapeModel.h"
#include "TinyCamera.h"
#include "PartModel.h"

class PartShapeViewer :
	public Viewer
{
public:
	shared_ptr<PartModel>	m_model;

	s3d::Array<s3d::Vec2>	m_verticles;
	
	s3d::Vec2*		m_selectedVerticle = nullptr;

	CursorCamera2D	m_camera;

public:
	PartShapeViewer(const shared_ptr<PartModel>& model)
		: m_model(model)
	{
		m_drawRect = s3d::RectF(0, 0, 600, 450);

		m_camera.setTargetCenter(s3d::Window::BaseSize() / 2 - m_drawRect.size / 2);
		m_camera.setCenter(s3d::Window::BaseSize() / 2 - m_drawRect.size / 2);
	}

	void	Init() override;
	void	Update() override;
};

