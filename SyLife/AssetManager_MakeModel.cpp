#include "AssetManager.h"

#include "Cell.h"
#include "Molecule.h"
#include "Body.h"
#include "Equipment.h"
#include "Module.h"
#include "Synthesizer.h"
#include "Wing.h"
#include "Needle.h"

shared_ptr<Model> AssetManager::makeModel(const string& name, const string& type)
{
	if (type == "CellModel")		return makeModel<CellModel>(name);
	if (type == "MoleculeModel")	return makeModel<MoleculeModel>(name);

	if (type == "BodyModel")		return makeModel<BodyModel>(name);
	if (type == "EquipmentModel")	return makeModel<EquipmentModel>(name);
	if (type == "ModuleModel")		return makeModel<ModuleModel>(name);

	if (type == "SynthesizerModel")	return makeModel<SynthesizerModel>(name);
	if (type == "WingModel")		return makeModel<WingModel>(name);
	if (type == "NeedleModel")		return makeModel<NeedleModel>(name);

	return nullptr;
}
