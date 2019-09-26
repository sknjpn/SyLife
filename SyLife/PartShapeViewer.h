#pragma once

#include "ModelViewer.h"
#include "TinyCamera.h"

class PartShapeViewer
	: public ModelViewer
{
public:
	Array<Vec2>	m_verticles;

	Vec2*		m_selectedVerticle = nullptr;

	TinyCamera	m_camera;

public:
	PartShapeViewer()
	{
		SetDrawRect(0, 0, 600, 450);

		m_camera.setControlOutOfScreenEnabled(false);
		m_camera.setRestrictedRect(none);
		m_camera.setTargetCenter(Scene::Size() / 2 - GetDrawRect().size / 2);
		m_camera.setCenter(Vec2::Zero());
	}

	void	Init() override;
	void	Update() override;
};

