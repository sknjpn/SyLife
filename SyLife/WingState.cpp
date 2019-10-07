#include "WingState.h"

#include "PartConfig.h"

#include "Cell.h"


void WingState::Flap(CellState& cell)
{
	cell.AddImpulseInLocal(Vec2::Up() * 10000.0, GetPartConfig()->GetPosition());
}
