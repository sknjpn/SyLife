#include "MoleculeManager.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

void MoleculeManager::Init()
{
	namespace fs = boost::filesystem;

	//s3d::Print << fs::path("\assets");
	fs::directory_iterator end;
	for (fs::directory_iterator it(fs::path("assets/molecule")); it != end; ++it)
	{
		if (!fs::is_directory(*it))
		{
			s3d::Print << s3d::Unicode::Widen( (*it).path().string());
			ImportModelFromFile((*it).path().string());
		}
	}
}