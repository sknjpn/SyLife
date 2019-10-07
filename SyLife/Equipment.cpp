#include "Equipment.h"
#include "ViewerManager.h"

void EquipmentModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<EquipmentViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}

shared_ptr<PartState> EquipmentModel::MakeState()
{
	return make_shared<EquipmentState>();
}
