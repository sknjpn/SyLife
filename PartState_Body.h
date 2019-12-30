#pragma once

#include "PartState.h"

class PartAsset_Body;

class PartState_Body
	: public PartState
{
	shared_ptr<PartAsset_Body> m_partAsset_Body;

public:
	PartState_Body(const shared_ptr<PartConfig>& partConfig);

	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;
};

