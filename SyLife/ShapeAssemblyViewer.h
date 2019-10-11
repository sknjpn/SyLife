#pragma once

#include "Viewer.h"
#include "TinyCamera.h"

class ShapeAssemblyViewer :
	public Viewer
{
	TinyCamera	m_camera;
	Vec2	m_size;

public:
	void	init() override;

	void	update() override;

	void	drawGrid() const;
};