#pragma once

#include "ModelViewer.h"
#include "TinyCamera.h"

class PartShapeViewer
	: public ModelViewer
{
public:
	Array<Vec2>	m_verticles;

	Vec2*		m_selectedVerticle = nullptr;

	CursorCamera2D	m_camera;

public:
	PartShapeViewer()
	{
		SetDrawRect(0, 0, 600, 450);

		m_camera.setTargetCenter(Window::BaseSize() / 2 - GetDrawRect().size / 2);
		m_camera.setCenter(Window::BaseSize() / 2 - GetDrawRect().size / 2);
	}

	void	Init() override;
	void	Update() override;
};

