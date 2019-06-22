#include "AssetManager.h"
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
