#pragma once

#include "Viewer.h"

class TitleViewer
	: public Viewer
{
	class Bubble
	{
	public:
		Vec3	m_position;
		double	m_timer = 0.0;
	};

	Array<Bubble>	m_bubbles;

public:
	TitleViewer();

	void	UpdateBubbles();
	void	drawBubbles();

	void	init() override;
	void	update() override;
};
