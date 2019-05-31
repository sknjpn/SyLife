#include "PartManager.h"
#include "Part.h"

shared_ptr<PartManager>	g_partManagerPtr;

PartManager::PartManager()
{
	m_models.reserve(10000);
}

PartManager::~PartManager()
{

}

const shared_ptr<Part::Model>& PartManager::AddModel()
{
	const auto& m = m_models.emplace_back(make_shared<Part::Model>());

	return m;
}

const shared_ptr<Part::Model>& PartManager::GetModel(const string & name) const
{
	for (auto it = m_models.begin(); it != m_models.end(); ++it)
	{
		if ((*it)->m_name == name) return *it;
	}

	throw new exception;
}
