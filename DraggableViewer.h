#pragma once

#include "Viewer.h"

class DraggableViewer :
	public Viewer
{
	bool	m_isGrabbed = false;
	Vec2	m_grabbedPos;
	Vec2	m_deltaPos;

public:
	void	update() override;
};

