#pragma once

#include "Viewer.h"

#include "TinyCamera.h"
#include "Curtain.h"

class CellAsset;

class FieldViewer
	: public Viewer
{
	TinyCamera	m_camera;
	Audio		m_audio;
	Curtain		m_openCurtain;
	
	shared_ptr<CellAsset>	m_newModel;

public:
	FieldViewer();

	void	init() override;

	void	update() override;

	const TinyCamera& getCamera() const { return m_camera; }
};
