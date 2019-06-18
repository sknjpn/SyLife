#include "AssetManager.h"
#include <boost/filesystem/operations.hpp>

#include "BodyModel.h"
#include "ModuleModel.h"
#include "EquipmentModel.h"

unique_ptr<AssetManager>	g_assetManagerPtr;

void AssetManager::Init()
{
	namespace fs = boost::filesystem;

	fs::directory_iterator end;
	
	for (fs::directory_iterator it(fs::path("assets/cell")); it != end; ++it)
		if (!fs::is_directory(*it)) g_assetManagerPtr->m_cellModels.emplace_back(make_shared<CellModel>())->Load((*it).path().string());
	
	for (fs::directory_iterator it(fs::path("assets/molecule")); it != end; ++it)
		if (!fs::is_directory(*it)) g_assetManagerPtr->m_moleculeModels.emplace_back(make_shared<MoleculeModel>())->Load((*it).path().string());
	
	for (fs::directory_iterator it(fs::path("assets/part/body")); it != end; ++it)
		if (!fs::is_directory(*it)) g_assetManagerPtr->m_partModels.emplace_back(make_shared<BodyModel>())->Load((*it).path().string());

	for (fs::directory_iterator it(fs::path("assets/part/equipment")); it != end; ++it)
		if (!fs::is_directory(*it)) g_assetManagerPtr->m_partModels.emplace_back(make_shared<EquipmentModel>())->Load((*it).path().string());

	for (fs::directory_iterator it(fs::path("assets/part/module")); it != end; ++it)
		if (!fs::is_directory(*it)) g_assetManagerPtr->m_partModels.emplace_back(make_shared<ModuleModel>())->Load((*it).path().string());
}
