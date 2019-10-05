#include "Chip.h"

void Chip::sendTo(const shared_ptr<Chip> chip, double rate)
{
	double value = rate;

	chip->m_nutrition += value;
	m_nutrition -= value;
}
