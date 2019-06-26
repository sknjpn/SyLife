#pragma once

class SystemManager
{
public:
	SystemManager();

	void	Update();
	void	Draw();
	void	Start();
};

extern unique_ptr<SystemManager>	g_systemManagerPtr;
