#include "SystemManager.h"

#include "ViewerManager.h"
#include "AssetViewer.h"

void Main()
{
	g_systemManagerPtr = make_unique<SystemManager>();

	g_viewerManagerPtr->AddViewer<AssetViewer>();

	g_systemManagerPtr->Start();

	while (s3d::System::Update())
	{
		g_systemManagerPtr->Update();
	}
}
