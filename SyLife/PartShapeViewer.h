#pragma once

#include "ModelViewer.h"

class PartShapeViewer
	: public ModelViewer
{
public:
	Array<Vec2>	m_verticles;

	Vec2*		m_selectedVerticle = nullptr;

	Camera2D	m_camera;

public:
	PartShapeViewer()
	{
		SetDrawRect(0, 0, 600, 450);

		m_camera.setTargetCenter(Scene::Size() / 2 - GetDrawRect().size / 2);
		m_camera.setCenter(Scene::Size() / 2 - GetDrawRect().size / 2);
	}

	void	Init() override;
	void	Update() override;
};

