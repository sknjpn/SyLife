#pragma once

class FieldManager
{
	Vec2	m_size;

public:
	void	init();
};

extern unique_ptr<FieldManager>	g_fieldManagerPtr;
