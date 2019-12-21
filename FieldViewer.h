#pragma once

#include "Viewer.h"

#include "TinyCamera.h"

class FieldViewer
	: public Viewer
{
	TinyCamera	m_camera;
	Audio		m_audio;

	void	openCellMakingViewer();

public:
	void	init() override;
	void	update() override;

	TinyCamera& getCamera() { return m_camera; }
	const TinyCamera& getCamera() const { return m_camera; }
};
