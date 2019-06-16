#include "SystemManager.h"

#include "ViewerManager.h"
#include "PartShapeViewer.h"
#include "BodyPropertyViewer.h"
#include "AssetViewer.h"

void Main()
{
	g_systemManagerPtr = make_unique<SystemManager>();
	
	g_viewerManagerPtr->AddViewer<PartShapeViewer>(s3d::RectF(0, 0, 600, 450));
	g_viewerManagerPtr->AddViewer<BodyPropertyViewer>(s3d::RectF(0, 450, 600, 150));
	g_viewerManagerPtr->AddViewer<AssetViewer>(s3d::RectF(600, 0, 200, 600));

	g_systemManagerPtr->Start();

	while (s3d::System::Update())
	{
		g_systemManagerPtr->Update();
	}
}
