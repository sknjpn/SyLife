#include "Wing.h"
#include "Cell.h"

void WingState::Flap(Cell& cell)
{
	cell.AddImpulseInLocal(Vector2D::Up()*100.0, m_config->m_position);
}
