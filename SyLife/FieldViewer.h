#pragma once

#include "Viewer.h"

#include "TinyCamera.h"
#include "Curtain.h"

#include "CellMakingViewer.h"
#include "CellStateViewer.h"
#include "StatisticsViewer.h"

class CellAsset;

class FieldViewer
	: public Viewer
{
	TinyCamera	m_camera;
	Audio		m_audio;
	Curtain		m_openCurtain;

	CellMakingViewer	m_cellMakingViewer;
	CellStateViewer		m_cellStateViewer;
	StatisticsViewer	m_statisticsViewer;
	
public:
	FieldViewer();

	void	update() override;

	const TinyCamera& getCamera() const { return m_camera; }
};
