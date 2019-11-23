#pragma once

#include "Viewer.h"

class WorldGenerateViewer :
	public Viewer
{
	void	generateWorld();

public:
	void	init() override;
	void	update() override;
};
