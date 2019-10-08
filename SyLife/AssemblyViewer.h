#pragma once

#include "TinyCamera.h"
#include "AssetEditor.h"

class AssemblyViewer
	: public AssetEditor
{
public:
	TinyCamera	m_camera;

	double	m_mass;
	double	m_inertia;
	double	m_radius;
	double	m_width = 300;
	double	m_height = 300;

public:
	AssemblyViewer();

	void	init() override;

	void	update() override;
};

