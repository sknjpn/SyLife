#pragma once

#include "Viewer.h"

class ScalingViewer :
	public Viewer
{
public:
	ScalingViewer()
	{
		setDrawSize(80, 320);
	}

	void	init() override;
	void	update() override;
};
