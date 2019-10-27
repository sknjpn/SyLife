#include "AddModelViewer.h"

#include "ElementAsset.h"
#include "CellAsset.h"
#include "PartAsset.h"
#include "BodyAsset.h"
#include "EquipmentAsset.h"
#include "ModuleAsset.h"
#include "WingAsset.h"

void AddModelViewer::update()
{
	drawAssets<ElementAsset>("ElementAsset");
	drawAssets<CellAsset>("CellAsset");
	drawAssets<PartAsset>("PartAsset");
	drawAssets<BodyAsset>("BodyAsset");
	drawAssets<EquipmentAsset>("EquipmentAsset");
	drawAssets<ModuleAsset>("ModuleAsset");
	drawAssets<WingAsset>("WingAsset");
}
