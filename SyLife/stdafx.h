#pragma once

#pragma warning(disable: 4100)

#include <Siv3D.hpp> // OpenSiv3D v0.3.1

#include <boost/range/numeric.hpp>

#include <boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

#include <boost/math/constants/constants.hpp>
using namespace boost::math::constants;


#include <boost/property_tree/ptree.hpp>
using namespace boost::property_tree;

#include <boost/filesystem/operations.hpp>
using namespace boost::filesystem;

using namespace std;
using namespace nanoflann;

// manager
#include "AssetManager.h"
#include "CellManager.h"
#include "EggManager.h"
#include "FieldManager.h"
#include "MoleculeManager.h"
#include "SystemManager.h"
#include "ViewerManager.h"
#include "WaveManager.h"
#include "TerrainManager.h"