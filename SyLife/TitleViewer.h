#pragma once

#include "Viewer.h"

#include "Curtain.h"

class TitleViewer
	: public Viewer
{
	class Bubble
	{
	public:
		Vec3	m_position;
		double	m_timer = 0.0;
	};

	enum Option
	{
		LaunchNewGame,
		ContinueGame,
		LaunchEditor,
		Exit,
	};

	Audio	m_audio;
	Option	m_selectedOption = Option::LaunchNewGame;
	Curtain m_closeCurtain;
	Curtain m_openCurtain;

	Array<Bubble>	m_bubbles;

public:
	TitleViewer();

	void	UpdateBubbles();
	void	drawBubbles();

	void	init() override;
	void	update() override;
};
