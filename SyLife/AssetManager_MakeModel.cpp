#include "AssetManager.h"

#include "CellModel.h"
#include "Molecule.h"
#include "BodyModel.h"
#include "EquipmentModel.h"
#include "ModuleModel.h"
#include "SynthesizerModel.h"
#include "WingModel.h"
#include "NeedleModel.h"

shared_ptr<Model> AssetManager::makeAsset(const string& name, const string& type)
{
	if (type == "CellModel")		return makeAsset<CellModel>(name);
	if (type == "MoleculeModel")	return makeAsset<MoleculeModel>(name);

	if (type == "BodyModel")		return makeAsset<BodyModel>(name);
	if (type == "EquipmentModel")	return makeAsset<EquipmentModel>(name);
	if (type == "ModuleModel")		return makeAsset<ModuleModel>(name);

	if (type == "SynthesizerModel")	return makeAsset<SynthesizerModel>(name);
	if (type == "WingModel")		return makeAsset<WingModel>(name);
	if (type == "NeedleModel")		return makeAsset<NeedleModel>(name);

	return nullptr;
}
