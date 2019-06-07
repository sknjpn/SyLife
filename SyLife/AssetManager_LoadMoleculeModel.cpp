#include "AssetManager.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

using namespace boost::property_tree;

void	AssetManager::LoadMoleculeModel(const string& filepath)
{
	const auto& model = g_assetManagerPtr->m_moleculeModels.emplace_back(make_shared<MoleculeModel>());

	ptree pt;
	read_json(filepath, pt);

	// Data.value
	if (boost::optional<double> value = pt.get_optional<double>("mass")) model->m_mass = value.get();
	else model->m_mass = 10.0;

	// Data.str
	if (boost::optional<std::string> str = pt.get_optional<std::string>("name")) model->m_name = str.get();
	else model->m_name = "str is nothing";

	model->m_radius = sqrt(model->m_mass);

	s3d::Logger << s3d::Unicode::Widen(model->m_name);
}