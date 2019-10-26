#pragma once

#include "Viewer.h"

class CellAsset;

class ReleaseViewer
	: public Viewer
{
	const shared_ptr<CellAsset>& getCellAsset() const;

public:
	ReleaseViewer();

	void	update() override;
};

