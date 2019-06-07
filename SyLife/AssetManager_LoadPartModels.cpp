#include "AssetManager.h"
#include <boost/foreach.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

void	AssetManager::LoadPartModels()
{
	namespace fs = boost::filesystem;

	const fs::path path("assets/part");

	BOOST_FOREACH(const fs::path& p, std::make_pair(fs::recursive_directory_iterator(path), fs::recursive_directory_iterator())) {
		if (!fs::is_directory(p)) LoadPartModel(p.string());
	}
}