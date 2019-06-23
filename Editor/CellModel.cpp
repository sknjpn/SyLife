#include "CellModel.h"

#include "ViewerManager.h"
#include "CellViewer.h"

void CellModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<CellViewer>(dynamic_pointer_cast<CellModel>(shared_from_this()));
}

ptree CellModel::AddToJSON(ptree pt) const
{
	// parts
	{
		ptree parts;

		for (const auto& e : m_parts)
			parts.push_back(std::make_pair("", e->Save()));

		pt.add_child("parts", parts);
	}

	return Model::AddToJSON(pt);
}

void CellModel::SetFromJSON(const ptree& pt)
{
	// parts
	for (auto part : pt.get_child("parts")) AddPartConfig(part.second);

	Model::SetFromJSON(pt);
}
