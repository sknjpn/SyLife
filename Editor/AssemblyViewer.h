#pragma once

#include "Viewer.h"
#include "CellModel.h"
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

#include "Viewer.h"

class AssemblyViewer :
	public Viewer
{
public:
	AssemblyViewer()
	{
		m_drawRect = s3d::RectF(0, 0, 600, 450);
	}

	void	Init() override;
	void	Update() override;
};

