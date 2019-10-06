#include "Terrain.h"

#include "ViewerManager.h"

void TerrainModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<TerrainViewer>()->SetModel(shared_from_this());
}
