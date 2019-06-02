#include "MoleculeManager.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

using namespace boost::property_tree;

void MoleculeManager::ImportModelFromFile(const string& filePath)
{
	const auto& molecule = g_moleculeManagerPtr->AddModel();

	ptree pt;
	read_json(filePath, pt);
	
	// Data.value
	if (boost::optional<double> value = pt.get_optional<double>("mass")) molecule->m_mass = value.get();
	else molecule->m_mass = 10.0;

	// Data.str
	if (boost::optional<std::string> str = pt.get_optional<std::string>("name")) molecule->m_name = str.get();
	else molecule->m_name = "str is nothing";

	molecule->m_radius = sqrt(molecule->m_mass);

	s3d::Logger << s3d::Unicode::Widen(molecule->m_name);
}
