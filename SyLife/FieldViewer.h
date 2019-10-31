#pragma once

#include "Viewer.h"

#include "TinyCamera.h"
#include "Curtain.h"

class FieldViewer
	: public Viewer
{
	TinyCamera	m_camera;
	Audio		m_audio;
	Curtain		m_openCurtain;

public:
	FieldViewer();

	void	init() override;
	void	update() override;

	const TinyCamera& getCamera() const { return m_camera; }
};
