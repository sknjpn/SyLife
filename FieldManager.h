#pragma once

class FieldManager
{
	Vec2	m_size;

public:
	void	init();

	const Vec2& getSize() const { return m_size; }
};

extern unique_ptr<FieldManager>	g_fieldManagerPtr;
