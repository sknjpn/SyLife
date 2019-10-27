#pragma once

#include "ModelEditor.h"
#include "TinyCamera.h"

class PartShapeViewer
	: public ModelEditor
{
public:
	Array<Vec2>	m_verticles;

	Vec2* m_selectedVerticle = nullptr;

	TinyCamera	m_camera;

public:
	PartShapeViewer()
	{
		setViewerRect(0, 0, 600, 600);

		m_camera.setControlOutOfScreenEnabled(false);
		m_camera.setRestrictedRect(RectF(-150, -150, 300, 300));
		m_camera.setScreen(getDrawRect());
		m_camera.setMaxScale(20.0);
		m_camera.setTargetCenter(Vec2::Zero());
		m_camera.setCenter(Vec2::Zero());
	}

	void	init() override;
	void	update() override;
};

