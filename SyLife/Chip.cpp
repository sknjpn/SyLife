#include "Chip.h"

Color Chip::getColor() const
{
	double r = 0.0, g = 0.0, b = 0.0;
	for (const auto& m : m_storage.GetMolecules())
	{
		r += m.first->GetColor().r / 255.0 * m.second / m_storage.Num();
		g += m.first->GetColor().g / 255.0 * m.second / m_storage.Num();
		b += m.first->GetColor().b / 255.0 * m.second / m_storage.Num();
	}

	return ColorF(r, g, b);
}
