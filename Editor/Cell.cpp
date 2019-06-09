#include "Cell.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

ptree CellModel::ToJSON() const
{
	ptree pt;

	// name
	pt.put("name", m_name);

	// body
	{

	}

	// modules
	{

	}

	// equipments
	{

	}

	return pt;
}

void CellModel::Load(const string& filepath)
{
	ptree pt;
	read_json(filepath, pt);

	// name
	m_name = pt.get<string>("name");
}
