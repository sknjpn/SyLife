#include "Module.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

ptree ModuleModel::ToJSON() const
{
	ptree pt;

	// name
	pt.put("name", m_name);

	// type
	pt.put("type", "Module");

	// mass
	pt.put<int>("mass", m_mass);

	return pt;
}

void ModuleModel::Load(const string& filepath)
{
	ptree pt;
	read_json(filepath, pt);

	// name
	m_name = pt.get<string>("name");

	// mass
	m_mass = pt.get<double>("mass");
}
