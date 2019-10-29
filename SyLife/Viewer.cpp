#include "Viewer.h"
#include "ViewerManager.h"

void Viewer::Run()
{
}

void Viewer::destroy()
{
	m_isDestroyed = true;

	for (auto& cv : m_childViewers)
		cv->destroy();
}

Array<shared_ptr<Viewer>> Viewer::getAllChildViewers() const
{
	Array<shared_ptr<Viewer>> result;

	result.emplace_back(m_childViewers);

	for (const auto& cv : m_childViewers)
		result.append(cv->getAllChildViewers());

	return result;
}
