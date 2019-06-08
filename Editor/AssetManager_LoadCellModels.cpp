#include "AssetManager.h"
#include <boost/foreach.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

void	AssetManager::LoadCellModels()
{
	namespace fs = boost::filesystem;

	s3d::Logger << U"\r--LoadCellModels";

	const fs::path path("assets/cell");

	BOOST_FOREACH(const fs::path& p, std::make_pair(fs::recursive_directory_iterator(path), fs::recursive_directory_iterator())) {
		if (!fs::is_directory(p)) LoadCellModel(p.string());
	}
}