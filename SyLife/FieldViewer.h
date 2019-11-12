#pragma once

#include "Viewer.h"

#include "TinyCamera.h"

class FieldViewer
	: public Viewer
{
	TinyCamera	m_camera;
	Audio		m_audio;

public:
	FieldViewer();

	void	init() override;
	void	update() override;

	const TinyCamera& getCamera() const { return m_camera; }
};
