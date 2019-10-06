#include "Equipment.h"
#include "ViewerManager.h"

void EquipmentModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<EquipmentViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}

shared_ptr<PartState> EquipmentModel::MakeState()
{
	return make_shared<EquipmentState>();
}
