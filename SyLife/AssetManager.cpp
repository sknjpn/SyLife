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

	for (fs::directory_iterator it(fs::path("assets/models")); it != end; ++it)
		if (!fs::is_directory(*it)) AddModel((*it).path().string());
}
