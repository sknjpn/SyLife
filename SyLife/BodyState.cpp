#include "BodyState.h"
#include "BodyModel.h"

#include "PartConfig.h"

void BodyState::Draw(const CellState& cell) const
{
	getPartConfig()->getModel()->Draw();
}

void BodyState::Update(CellState& cell)
{
}
