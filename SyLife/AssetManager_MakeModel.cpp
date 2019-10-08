#include "AssetManager.h"

#include "CellAsset.h"
#include "ElementAsset.h"
#include "BodyAsset.h"
#include "EquipmentAsset.h"
#include "ModuleAsset.h"
#include "SynthesizerAsset.h"
#include "WingAsset.h"
#include "NeedleAsset.h"

shared_ptr<Asset> AssetManager::makeAsset(const string& type)
{
	if (type == "CellAsset")		return makeAsset<CellAsset>();
	if (type == "ElementAsset")		return makeAsset<ElementAsset>();

	if (type == "BodyAsset")		return makeAsset<BodyAsset>();
	if (type == "EquipmentAsset")	return makeAsset<EquipmentAsset>();
	if (type == "ModuleAsset")		return makeAsset<ModuleAsset>();

	if (type == "SynthesizerAsset")	return makeAsset<SynthesizerAsset>();
	if (type == "WingAsset")		return makeAsset<WingAsset>();
	if (type == "NeedleAsset")		return makeAsset<NeedleAsset>();

	return nullptr;
}
