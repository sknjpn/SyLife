#include "SystemManager.h"

void Main()
{
	g_systemManagerPtr = make_unique<SystemManager>();

	while (s3d::System::Update())
	{
		g_systemManagerPtr->Update();
	}
}
