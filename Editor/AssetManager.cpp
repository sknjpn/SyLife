#include "AssetManager.h"
#include <boost/filesystem/operations.hpp>

#include "BodyModel.h"
#include "ModuleModel.h"
#include "EquipmentModel.h"

unique_ptr<AssetManager>	g_assetManagerPtr;

void AssetManager::Init()
{
	namespace fs = boost::filesystem;

	auto makept = [](const fs::directory_iterator& it) { ptree pt; read_json((*it).path().string(), pt); return pt; };

	fs::directory_iterator end;

	for (fs::directory_iterator it(fs::path("assets/molecule")); it != end; ++it)
		if (!fs::is_directory(*it)) m_moleculeModels.emplace_back(make_shared<MoleculeModel>())->Load(makept(it));

	for (fs::directory_iterator it(fs::path("assets/part")); it != end; ++it)
		if (!fs::is_directory(*it)) m_partModels.emplace_back(m_bodyModels.emplace_back(make_shared<BodyModel>()))->Load(makept(it));

	for (fs::directory_iterator it(fs::path("assets/part")); it != end; ++it)
		if (!fs::is_directory(*it)) m_partModels.emplace_back(m_equipmentModels.emplace_back(make_shared<EquipmentModel>()))->Load(makept(it));

	for (fs::directory_iterator it(fs::path("assets/part")); it != end; ++it)
		if (!fs::is_directory(*it)) m_partModels.emplace_back(m_moduleModels.emplace_back(make_shared<ModuleModel>()))->Load(makept(it));

	for (fs::directory_iterator it(fs::path("assets/cell")); it != end; ++it)
		if (!fs::is_directory(*it)) m_cellModels.emplace_back(make_shared<CellModel>())->Load(makept(it));
}
