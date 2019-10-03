#pragma once

class Chip;

class FieldManager
{
	Grid<shared_ptr<Chip>> m_chips;

public:
	void	Init();
};

extern unique_ptr<FieldManager>	g_fieldManagerPtr;
