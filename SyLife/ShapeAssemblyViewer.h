#pragma once

#include "Viewer.h"
#include "TinyCamera.h"

class ShapeAssemblyViewer :
	public Viewer
{
	TinyCamera	m_camera;

	double	m_width = 300;
	double	m_height = 300;

public:
	void	init() override;

	void	update() override;

	void	drawGrid() const;
};