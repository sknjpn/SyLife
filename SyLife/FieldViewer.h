#pragma once

#include "Viewer.h"

#include "TinyCamera.h"
#include "Curtain.h"

class CellMakingButton;
class CellMakingViewer;
class CellStateViewer;
class StatisticsViewer;

class FieldViewer
	: public Viewer
{
	TinyCamera	m_camera;
	Audio		m_audio;
	Curtain		m_openCurtain;

	unique_ptr<CellMakingButton>	m_cellMakingButton;
	unique_ptr<CellMakingViewer>	m_cellMakingViewer;
	unique_ptr<CellStateViewer>		m_cellStateViewer;
	unique_ptr<StatisticsViewer>	m_statisticsViewer;
	
public:
	FieldViewer();

	void	update() override;

	const TinyCamera& getCamera() const { return m_camera; }
};
