#include "BodyState.h"
#include "BodyAsset.h"

#include "PartConfig.h"

void BodyState::draw(const CellState& cellState) const
{
	getPartConfig()->getPartAsset()->draw();
}

void BodyState::update(CellState& cellState)
{
}
