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
}

