#include "CellModel.h"
#include <boost/property_tree/json_parser.hpp>


void CellModel::FromJSON(const ptree& pt)
{
	// name
	m_name = pt.get<string>("name");
}
