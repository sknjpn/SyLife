#include "Chip.h"

Color Chip::getColor() const
{
	double r, g, b = 0.0;
	for (const auto& m : m_storage.GetMolecules())
	{
		m.first->GetColor()* m_storage.Num();
	}

	return Color();
}
