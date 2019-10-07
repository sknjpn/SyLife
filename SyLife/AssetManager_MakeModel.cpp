#include "AssetManager.h"

#include "Cell.h"
#include "Molecule.h"
#include "Body.h"
#include "Equipment.h"
#include "Module.h"
#include "Eye.h"
#include "Chloroplast.h"
#include "Synthesizer.h"
#include "Wing.h"
#include "Needle.h"
#include "Hotspot.h"
#include "Terrain.h"

shared_ptr<Model> AssetManager::makeModel(const string& name, const string& type)
{
	if (type == "CellModel")		return makeModel<CellModel>(name);
	if (type == "MoleculeModel")	return makeModel<MoleculeModel>(name);

	if (type == "BodyModel")		return makeModel<BodyModel>(name);
	if (type == "EquipmentModel")	return makeModel<EquipmentModel>(name);
	if (type == "ModuleModel")		return makeModel<ModuleModel>(name);

	if (type == "EyeModel")			return makeModel<EyeModel>(name);
	if (type == "ChloroplastModel")	return makeModel<ChloroplastModel>(name);
	if (type == "SynthesizerModel")	return makeModel<SynthesizerModel>(name);
	if (type == "WingModel")		return makeModel<WingModel>(name);
	if (type == "NeedleModel")		return makeModel<NeedleModel>(name);

	if (type == "HotspotModel")		return makeModel<HotspotModel>(name);
	if (type == "TerrainModel")		return makeModel<TerrainModel>(name);

	return nullptr;
}
