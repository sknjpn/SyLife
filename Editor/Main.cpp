#include "SystemManager.h"

#include "ViewerManager.h"
#include "PartShapeViewer.h"
#include "BodyPropertyViewer.h"
#include "AssetViewer.h"

void Main()
{
	g_systemManagerPtr = make_unique<SystemManager>();

	g_viewerManagerPtr->AddViewer<AssetViewer>(s3d::RectF(600, 0, 200, 600));

	g_systemManagerPtr->Start();

	while (s3d::System::Update())
	{
		g_systemManagerPtr->Update();
	}
}
