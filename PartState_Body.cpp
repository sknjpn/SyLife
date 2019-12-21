#include "PartState_Body.h"
#include "PartAsset_Body.h"
#include "PartConfig.h"

void PartState_Body::draw(const CellState& cellState) const
{
	getPartConfig()->getPartAsset()->draw();
}

void PartState_Body::update(CellState& cellState)
{
}
