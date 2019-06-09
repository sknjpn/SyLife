#include "Cell.h"

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

void CellModel::Load(const string & path) const
{
}

void CellModel::Save() const
{
}
