#pragma once

#include "Viewer.h"

class CellAsset;

class ReleaseViewer
	: public Viewer
{
	shared_ptr<CellAsset>	m_cellAsset;

public:
	ReleaseViewer(const shared_ptr<CellAsset>& cellAsset);

	void	update() override;

	bool	isReleased() const { return !m_cellAsset; }
};

