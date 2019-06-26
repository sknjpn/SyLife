#pragma once

#include "Viewer.h"

class TitleViewer
	: public Viewer
{
	class Bubble
	{
	public:
		s3d::Vec3	m_position;
		double		m_timer = 0.0;
	};

	s3d::Array<Bubble>	m_bubbles;
	s3d::Audio m_audio;

public:
	TitleViewer()
		: m_audio(U"assets/music/ê_îÈÇÃê¢äE.mp3")
	{
		m_drawRect = s3d::RectF(s3d::Window::Size());
		m_audio.setLoop(true);
		m_audio.play();
	}

	void	UpdateBubbles();
	void	DrawBubbles();

	void	Init() override;
	void	Update(bool	isMouseOver) override;
};

