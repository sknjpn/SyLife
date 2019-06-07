#include "PartManager.h"
#include "Part.h"

shared_ptr<PartManager>	g_partManagerPtr;

const shared_ptr<PartModel>& PartManager::GetModel(const string & name) const
{
	for (auto it = m_models.begin(); it != m_models.end(); ++it)
	{
		if ((*it)->m_name == name) return *it;
	}

	throw new exception;
}
