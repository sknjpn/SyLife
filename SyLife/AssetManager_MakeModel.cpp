#include "AssetManager.h"

#include "CellAsset.h"
#include "Element.h"
#include "BodyAsset.h"
#include "EquipmentAsset.h"
#include "ModuleAsset.h"
#include "SynthesizerAsset.h"
#include "WingAsset.h"
#include "NeedleAsset.h"

shared_ptr<Asset> AssetManager::makeAsset(const string& name, const string& type)
{
	if (type == "CellAsset")		return makeAsset<CellAsset>(name);
	if (type == "ElementAsset")	return makeAsset<ElementAsset>(name);

	if (type == "BodyAsset")		return makeAsset<BodyAsset>(name);
	if (type == "EquipmentAsset")	return makeAsset<EquipmentAsset>(name);
	if (type == "ModuleAsset")		return makeAsset<ModuleAsset>(name);

	if (type == "SynthesizerAsset")	return makeAsset<SynthesizerAsset>(name);
	if (type == "WingAsset")		return makeAsset<WingAsset>(name);
	if (type == "NeedleAsset")		return makeAsset<NeedleAsset>(name);

	return nullptr;
}
