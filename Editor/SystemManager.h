#pragma once

class SystemManager
{
public:
	SystemManager();

	void	Update();
};

extern unique_ptr<SystemManager>	g_systemManagerPtr;