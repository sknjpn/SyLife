#pragma once

#include "ViewerManager.h"

class SystemManager
{
public:
	SystemManager();

	void	Update()
	{
		g_viewerManagerPtr->Update();
	}
};

extern unique_ptr<SystemManager>	g_systemManagerPtr;
