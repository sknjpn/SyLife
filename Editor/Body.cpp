#include "Body.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

ptree BodyModel::ToJSON() const
{
	ptree pt;

	// name
	pt.put("name", m_name);

	// type
	pt.put("type", "Body");

	// mass
	pt.put<int>("mass", m_mass);

	return pt;
}

void BodyModel::Load(const string& filepath)
{
	ptree pt;
	read_json(filepath, pt);

	// name
	m_name = pt.get<string>("name");

	// mass
	m_mass = pt.get<double>("mass");
}

void BodyModel::Save() const
{

}

