#pragma once

#include "Viewer.h"
#include "TinyCamera.h"

class FieldViewer
	: public Viewer
{
	s3d::Audio	m_audio;
	RestrictedCamera2D	m_cursorCamera2D;

public:
	FieldViewer()
		: m_audio(U"assets/music/genomics_analysis.mp3")
	{
		m_drawRect = s3d::RectF(s3d::Window::Size());
		m_audio.setLoop(true);
		m_audio.play();
	}
	~FieldViewer()
	{

	}

	void	Init() override;
	void	Update() override;
};

