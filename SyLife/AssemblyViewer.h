#pragma once

#include "Viewer.h"
#include "Cell.h"
#include "TinyCamera.h"
#include "Part.h"

class AssemblyViewer
	: public Viewer
{
	shared_ptr<CellModel>	m_model;

	CursorCamera2D	m_camera;

	double	m_mass;
	double	m_inertia;
	double	m_radius;

public:
	AssemblyViewer(const shared_ptr<CellModel>& model)
		: m_model(model)
	{
		m_drawRect = s3d::RectF(s3d::Window::Width() - 800, 50, 600, 600);

		m_camera.setTargetCenter(s3d::Window::BaseSize() / 2 - m_drawRect.size / 2);
		m_camera.setCenter(s3d::Window::BaseSize() / 2 - m_drawRect.size / 2);
	}

	void	Init() override;
	void	Update() override;

	void	CalculateDisk();
};

