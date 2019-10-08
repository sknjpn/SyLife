#include "BodyState.h"
#include "BodyAsset.h"

#include "PartConfig.h"

void BodyState::draw(const CellState& cell) const
{
	getPartConfig()->getModel()->draw();
}

void BodyState::update(CellState& cell)
{
}
