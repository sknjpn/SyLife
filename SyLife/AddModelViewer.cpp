#include "AddModelViewer.h"

#include "Element.h"
#include "CellAsset.h"
#include "PartAsset.h"
#include "BodyAsset.h"
#include "EquipmentAsset.h"
#include "ModuleAsset.h"
#include "WingAsset.h"

void AddModelViewer::update()
{
	DrawModels<ElementAsset>("ElementAsset");
	DrawModels<CellAsset>("CellAsset");
	DrawModels<PartAsset>("PartAsset");
	DrawModels<BodyAsset>("BodyAsset");
	DrawModels<EquipmentAsset>("EquipmentAsset");
	DrawModels<ModuleAsset>("ModuleAsset");
	DrawModels<WingAsset>("WingAsset");
}
