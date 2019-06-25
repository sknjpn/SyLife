#include "Model.h"

void Model::SetFromJSON(const ptree& pt)
{
	m_name = pt.get<string>("name");
}
