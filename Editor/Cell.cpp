#include "Cell.h"

#include "ViewerManager.h"

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
	for (auto part : pt.get_child("parts")) m_parts.emplace_back(make_shared<PartConfig>())->Load(pt);

	Model::SetFromJSON(pt);
}

void CellViewer::Update()
{
	// name
	{
		s3d::SimpleGUI::TextBox(m_textEditState_name, s3d::Vec2(10, 10), 240);
		m_model->m_name = s3d::Unicode::Narrow(m_textEditState_name.text);
	}
}
