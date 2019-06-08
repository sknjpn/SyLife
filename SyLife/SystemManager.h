#pragma once

class SystemManager
{
public:
	SystemManager();

	void	Update();
	void	Draw();
};

extern unique_ptr<SystemManager>	g_systemManagerPtr;
