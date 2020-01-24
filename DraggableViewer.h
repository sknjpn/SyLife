#pragma once

#include "EasyViewer.h"

class DraggableViewer :
	public EasyViewer
{
	bool	m_isGrabbed = false;
	Vec2	m_grabbedPos;
	Vec2	m_deltaPos;

public:
	void	update() override;
};

