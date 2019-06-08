#include "AssetManager.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

using namespace boost::property_tree;

void	AssetManager::LoadCellModel(const string& filepath)
{
	ptree pt;
	read_json(filepath, pt);

	const auto& model = g_assetManagerPtr->m_cellModels.emplace_back(make_shared<CellModel>());

	// name
	model->m_name = pt.get<string>("name");

	s3d::Logger << s3d::Unicode::Widen(model->m_name);
}