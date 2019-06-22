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

const shared_ptr<MoleculeModel>& AssetManager::GetMoleculeModel(const string& name) const
{
	for (auto it = m_moleculeModels.begin(); it != m_moleculeModels.end(); ++it)
		if ((*it)->m_name == name) return *it;

	throw new exception;
}

const shared_ptr<PartModel>& AssetManager::GetPartModel(const string& name) const
{
	for (auto it = m_partModels.begin(); it != m_partModels.end(); ++it)
		if ((*it)->m_name == name) return *it;

	throw new exception;
}

const shared_ptr<BodyModel>& AssetManager::GetBodyModel(const string& name) const
{
	for (auto it = m_bodyModels.begin(); it != m_bodyModels.end(); ++it)
		if ((*it)->m_name == name) return *it;

	throw new exception;
}

const shared_ptr<EquipmentModel>& AssetManager::GetEquipmentModel(const string& name) const
{
	for (auto it = m_equipmentModels.begin(); it != m_equipmentModels.end(); ++it)
		if ((*it)->m_name == name) return *it;

	throw new exception;
}

const shared_ptr<ModuleModel>& AssetManager::GetModuleModel(const string& name) const
{
	for (auto it = m_moduleModels.begin(); it != m_moduleModels.end(); ++it)
		if ((*it)->m_name == name) return *it;

	throw new exception;
}

const shared_ptr<CellModel>& AssetManager::GetCellModel(const string& name) const
{
	for (auto it = m_cellModels.begin(); it != m_cellModels.end(); ++it)
		if ((*it)->m_name == name) return *it;

	throw new exception;
}
