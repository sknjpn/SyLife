#pragma once

#include "Viewer.h"

class MainViewer :
	public Viewer
{
	void	openCellMakingViewer();

public:
	void	init() override;
	void	update() override;
};

