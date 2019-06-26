#pragma once

class SystemManager
{
public:
	SystemManager();

	void	Update();
	void	Start();
};

extern unique_ptr<SystemManager>	g_systemManagerPtr;
