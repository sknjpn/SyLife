#pragma once

class Chip;

class FieldManager
{
	Grid<shared_ptr<Chip>> m_chips;

public:
	void	Init();

	const shared_ptr<Chip>& GetChip(const Vec2& position) const;
};

extern unique_ptr<FieldManager>	g_fieldManagerPtr;
