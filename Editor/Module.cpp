#include "Module.h"

ptree ModuleModel::ToJSON() const
{
	ptree pt;

	// name
	pt.put("name", m_name);

	// type
	pt.put("type", "Module");

	// mass
	pt.put<int>("mass", m_mass);
}
