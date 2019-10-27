#pragma once

#include "Viewer.h"

class ScalingViewer :
	public Viewer
{
	double	m_value = 0.0;

public:
	void	init() override;
	void	update() override;
};
