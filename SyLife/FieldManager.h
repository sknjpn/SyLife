#pragma once

class FieldManager
{
public:
	constexpr double	GetDeltaTime() const { return 1.0 / 60.0; }

	void	Init();
	void	Update();
};

extern unique_ptr<FieldManager>	g_fieldManagerPtr;
