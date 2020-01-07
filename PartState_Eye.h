#pragma once

#include "PartState.h"

class PartAsset_Eye;

class PartState_Eye
	: public PartState
{
	shared_ptr<PartAsset_Eye> m_partAsset_Eye;

public:
	PartState_Eye(const shared_ptr<PartConfig>& partConfig);

	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;
};
