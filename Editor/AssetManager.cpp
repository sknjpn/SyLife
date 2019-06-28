#include "AssetManager.h"
#include "Model.h"
#include <boost/filesystem/operations.hpp>

unique_ptr<AssetManager>	g_assetManagerPtr;

void AssetManager::Init()
{
	AddModels("assets/models/molecules");
	AddModels("assets/models/parts");
	AddModels("assets/models/cells");
}

void AssetManager::AddModels(const string& directory)
{
	namespace fs = boost::filesystem;

	fs::directory_iterator end;

	for (fs::directory_iterator it(fs::path(directory.c_str())); it != end; ++it)
		if (!fs::is_directory(*it)) AddModel((*it).path().string());
}

const shared_ptr<Model>& AssetManager::GetModel(const string& name) const
{
	for (auto it = m_models.begin(); it != m_models.end(); ++it)
		if ((*it)->m_name == name && dynamic_pointer_cast<Model>(*it) != nullptr) return dynamic_pointer_cast<Model>(*it);

	return nullptr;
}
