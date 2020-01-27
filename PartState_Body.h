#pragma once

#include "PartState.h"

class PartAsset_Body;

class PartState_Body
	: public PartState
{
	std::shared_ptr<PartAsset_Body> m_partAsset_Body;

public:
	PartState_Body(const std::shared_ptr<PartConfig>& partConfig);

	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;
};

