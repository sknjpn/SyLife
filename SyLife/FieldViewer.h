#pragma once

#include "Viewer.h"
#include "TinyCamera.h"

class CellModel;
class PartPaletteViewer;
class AssemblyViewer;
class ReleaseViewer;

class FieldViewer
	: public Viewer
{
	s3d::Audio	m_audio;
	RestrictedCamera2D	m_cursorCamera2D;
	shared_ptr<CellModel>	m_newModel;
	shared_ptr<PartPaletteViewer>	m_partPaletteViewer;
	shared_ptr<AssemblyViewer>		m_assemblyViewer;
	shared_ptr<ReleaseViewer>		m_releaseViewer;

public:
	FieldViewer()
		: m_audio(U"assets/music/AQUA.mp3")
	{
		m_drawRect = s3d::RectF(s3d::Window::Size());
		m_audio.setLoop(true);
		m_audio.play();
	}

	void	Init() override;
	void	Update(bool isMouseOver) override;
};

