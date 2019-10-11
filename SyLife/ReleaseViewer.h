#pragma once

#include "Viewer.h"

class CellAsset;

class ReleaseViewer
	: public Viewer
{
public:
	ReleaseViewer();

	void	update() override;
};

