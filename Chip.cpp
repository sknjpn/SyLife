#include "Chip.h"

void Chip::sendTo(const shared_ptr<Chip>& chip, double value)
{
	chip->m_nutrition += value;
	m_nutrition -= value;
}
