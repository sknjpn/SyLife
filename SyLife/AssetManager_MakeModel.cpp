#include "AssetManager.h"

#include "CellAsset.h"
#include "ElementAsset.h"
#include "BodyAsset.h"
#include "EquipmentAsset.h"
#include "ModuleAsset.h"
#include "SynthesizerAsset.h"
#include "WingAsset.h"
#include "NeedleAsset.h"

shared_ptr<Asset> AssetManager::makeAsset(const string& name, const string& type)
{
	shared_ptr<Asset> asset;

	if (type == "CellAsset")		asset = makeAsset<CellAsset>();
	if (type == "ElementAsset")		asset = makeAsset<ElementAsset>();

	if (type == "BodyAsset")		asset = makeAsset<BodyAsset>();
	if (type == "EquipmentAsset")	asset = makeAsset<EquipmentAsset>();
	if (type == "ModuleAsset")		asset = makeAsset<ModuleAsset>();

	if (type == "SynthesizerAsset")	asset = makeAsset<SynthesizerAsset>();
	if (type == "WingAsset")		asset = makeAsset<WingAsset>();
	if (type == "NeedleAsset")		asset = makeAsset<NeedleAsset>();

	if (asset) asset->setName(name);

	return asset;
}
