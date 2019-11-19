#pragma once

#include "Viewer.h"

class SetupViewer
	: public Viewer
{
	void	startGame();

public:
	void	init() override;
	void	update() override;
};

