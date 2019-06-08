#pragma once

#include "Viewer.h"
#include "Shape.h"

class PartViewer :
	public Viewer
{
public:
	s3d::Array<Shape> shapes;
	s3d::Array<s3d::Vec2> verticles;
	s3d::Vec2* selectedVerticle = nullptr;

public:
	void	Update() override;
};

