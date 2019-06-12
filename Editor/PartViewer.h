#pragma once

#include "Viewer.h"
#include "Shape.h"
#include "TinyCamera.h"

class PartViewer :
	public Viewer
{
public:
	s3d::Array<Shape> shapes;
	s3d::Array<s3d::Vec2> verticles;
	s3d::Vec2* selectedVerticle = nullptr;
	CursorCamera2D m_camera;

public:
	void	Init();
	void	Update() override;

	void	Save();
};

