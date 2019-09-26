#pragma once

#include "ModelViewer.h"
#include "TinyCamera.h"

class PartShapeViewer
	: public ModelViewer
{
public:
	Array<Vec2>	m_verticles;

	Vec2* m_selectedVerticle = nullptr;

	TinyCamera	m_camera;

public:
	PartShapeViewer()
	{
		SetDrawRect(0, 0, 600, 600);

		m_camera.setControlOutOfScreenEnabled(false);
		m_camera.setRestrictedRect(RectF(-150, -150, 300, 300));
		m_camera.setScreen(GetDrawRect());
		m_camera.setMaxScale(20.0);
		m_camera.setTargetCenter(Vec2::Zero());
		m_camera.setCenter(Vec2::Zero());
	}

	void	Init() override;
	void	Update() override;
};

