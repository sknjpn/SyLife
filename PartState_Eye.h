#pragma once

#include "PartState.h"

class PartAsset_Eye;

class PartState_Eye
	: public PartState
{
	std::shared_ptr<PartAsset_Eye> m_partAsset_Eye;
	double	m_heat = 0.0;
	std::shared_ptr<CellState>	m_targetCellState;

public:
	PartState_Eye(const std::shared_ptr<PartConfig>& partConfig);
	
	const std::shared_ptr<PartAsset_Eye>& getPartAsset_Eye() const { return m_partAsset_Eye; }
	const std::shared_ptr<CellState>& getTargetCellState() const { return m_targetCellState; }

	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;
};
