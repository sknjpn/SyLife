#pragma once

class FieldManager
{
	Vec2	m_size;

public:
	void	Init();
};

extern unique_ptr<FieldManager>	g_fieldManagerPtr;
