#pragma once

#include "Viewer.h"
#include "TinyCamera.h"

class CellAsset;

class AssemblyViewer
	: public Viewer
{
public:
	TinyCamera	m_camera;

	double	m_mass;
	double	m_inertia;
	double	m_radius;
	double	m_width = 300;
	double	m_height = 300;

	shared_ptr<CellAsset>	m_cellAsset;

public:
	AssemblyViewer();

	void	init() override;

	void	update() override;
};

