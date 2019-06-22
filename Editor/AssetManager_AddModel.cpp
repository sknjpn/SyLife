#include "AssetManager.h"

#include "CellModel.h"

#include "MoleculeModel.h"

#include "BodyModel.h"
#include "EquipmentModel.h"
#include "ModuleModel.h"

#include "EyeModel.h"
#include "ChloroplastModel.h"
#include "WingModel.h"

void AssetManager::AddModel(const string& filepath)
{
	ptree pt;

	read_json(filepath, pt);

	// 値が存在しない場合(Modelファイルでない場合)
	if (!pt.get_optional<string>("type")) return;

	auto type = pt.get<string>("type");

	if (type == "Model")			AddModel<Model>(pt);
	if (type == "CellModel")		AddModel<CellModel>(pt);
	if (type == "MoleculeModel")	AddModel<MoleculeModel>(pt);

	if (type == "BodyModel")		AddModel<BodyModel>(pt);
	if (type == "EquipmentModel")	AddModel<EquipmentModel>(pt);
	if (type == "ModuleModel")		AddModel<ModuleModel>(pt);

	if (type == "EyeModel")			AddModel<EyeModel>(pt);
	if (type == "ChloroplastModel")	AddModel<ChloroplastModel>(pt);
	if (type == "WingModel")		AddModel<WingModel>(pt);
}
