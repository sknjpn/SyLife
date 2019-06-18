#include "CellModel.h"
#include <boost/property_tree/json_parser.hpp>

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

void CellModel::FromJSON(const ptree& pt)
{
	// name
	m_name = pt.get<string>("name");
}
