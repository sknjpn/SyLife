#pragma once

#include "Viewer.h"

class PartAsset;

class ShapeLayerViewer :
	public Viewer
{
	shared_ptr<PartAsset>	m_partAsset;

public:
	void	init() override;
	void	update() override;

	void	setPartAsset(const shared_ptr<PartAsset>& partAsset) { m_partAsset = partAsset; }
};

