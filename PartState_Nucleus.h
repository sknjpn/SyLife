#pragma once

#include "PartState.h"

class PartAsset_Nucleus;

class PartState_Nucleus
	: public PartState
{
	std::shared_ptr<PartAsset_Nucleus> m_partAsset_Nucleus;

public:
	PartState_Nucleus(const std::shared_ptr<PartConfig>& partConfig);

	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;
};
