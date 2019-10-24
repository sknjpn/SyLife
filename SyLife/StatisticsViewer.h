#pragma once

#include "Viewer.h"

class StatisticsViewer :
	public Viewer
{
public:
	void	init() override;
	void	update() override;
};

