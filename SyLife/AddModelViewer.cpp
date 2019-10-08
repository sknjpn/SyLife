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
	drawModels<ElementAsset>("ElementAsset");
	drawModels<CellAsset>("CellAsset");
	drawModels<PartAsset>("PartAsset");
	drawModels<BodyAsset>("BodyAsset");
	drawModels<EquipmentAsset>("EquipmentAsset");
	drawModels<ModuleAsset>("ModuleAsset");
	drawModels<WingAsset>("WingAsset");
}
