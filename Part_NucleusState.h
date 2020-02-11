#pragma once

#include "PartState.h"

class Part_NucleusAsset;

class PartState_Nucleus
	: public PartState
{
	std::shared_ptr<Part_NucleusAsset> m_Part_NucleusAsset;

public:
	PartState_Nucleus(const std::shared_ptr<PartConfig>& partConfig);

	void	update(CellState& cellState) override;
};
