#pragma once

class FieldManager
{
public:
	void	Init();
	void	Update();
};

extern unique_ptr<FieldManager>	g_fieldManagerPtr;
