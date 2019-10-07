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

shared_ptr<Model> AssetManager::MakeModel(const string& name, const string& type)
{
	if (type == "CellModel")		return MakeModel<CellModel>(name);
	if (type == "MoleculeModel")	return MakeModel<MoleculeModel>(name);

	if (type == "BodyModel")		return MakeModel<BodyModel>(name);
	if (type == "EquipmentModel")	return MakeModel<EquipmentModel>(name);
	if (type == "ModuleModel")		return MakeModel<ModuleModel>(name);

	if (type == "EyeModel")			return MakeModel<EyeModel>(name);
	if (type == "ChloroplastModel")	return MakeModel<ChloroplastModel>(name);
	if (type == "SynthesizerModel")	return MakeModel<SynthesizerModel>(name);
	if (type == "WingModel")		return MakeModel<WingModel>(name);
	if (type == "NeedleModel")		return MakeModel<NeedleModel>(name);

	if (type == "HotspotModel")		return MakeModel<HotspotModel>(name);
	if (type == "TerrainModel")		return MakeModel<TerrainModel>(name);

	return nullptr;
}
