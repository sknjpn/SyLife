#pragma once

#include "Viewer.h"

class ScalingViewer :
	public Viewer
{
public:
	void	init() override;
	void	update() override;
};
