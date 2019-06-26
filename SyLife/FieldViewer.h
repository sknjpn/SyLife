#pragma once

#include "Viewer.h"

class FieldViewer
	: public Viewer
{
	s3d::Audio m_audio;
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

