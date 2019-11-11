#include "AddModelViewer.h"

#include "ElementAsset.h"
#include "CellAsset.h"
#include "PartAsset.h"
#include "BodyAsset.h"
#include "EquipmentAsset.h"
#include "ModuleAsset.h"
#include "NucleusAsset.h"
#include "WingAsset.h"

void AddModelViewer::update()
{
	drawAssets<ElementAsset>(U"ElementAsset");
	drawAssets<CellAsset>(U"CellAsset");
	drawAssets<PartAsset>(U"PartAsset");
	drawAssets<BodyAsset>(U"BodyAsset");
	drawAssets<EquipmentAsset>(U"EquipmentAsset");
	drawAssets<ModuleAsset>(U"ModuleAsset");
	drawAssets<NucleusAsset>(U"NucleusAsset");
	drawAssets<WingAsset>(U"WingAsset");
}

void AddModelViewer::init()
{
	setViewerRect(100, 150, 400, 300);
}
