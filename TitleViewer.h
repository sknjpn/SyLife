#pragma once

#include "EasyViewer.h"

class TitleViewer
	: public EasyViewer
{
	class Bubble
	{
	public:
		Vec3	m_position;
		double	m_timer = 0.0;
	};

	Audio	m_audio;

	Array<Bubble>	m_bubbles;

public:
	TitleViewer();

	void	UpdateBubbles();
	void	drawBubbles();

	void	runNew();
	void	runContinue();

	void	init() override;
	void	update() override;
};
