#pragma once

#include "DraggableViewer.h"

class CellAsset;

class CellAssetViewer
	: public DraggableViewer
{
	shared_ptr<CellAsset>	m_cellAsset;

public:
	CellAssetViewer(const shared_ptr<CellAsset>& cellAsset);

	// get
	const shared_ptr<CellAsset>& getCellAsset() const { return m_cellAsset; }

	void	update() override;
};

