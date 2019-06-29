#include "Cell.h"

#include "ViewerManager.h"

void CellModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<CellViewer>(dynamic_pointer_cast<CellModel>(shared_from_this()));
}

void CellModel::AddToJSON(ptree& pt) const
{
	// parts
	{
		ptree parts;

		for (const auto& e : m_partConfigs)
		{
			ptree part; e->Save(part);
			parts.push_back(std::make_pair("", part));
		}

		pt.add_child("parts", parts);
	}

	Model::AddToJSON(pt);

	pt.put("type", "CellModel");
}

void CellModel::SetFromJSON(const ptree& pt)
{
	// parts
	for (auto part : pt.get_child("parts")) m_partConfigs.emplace_back(make_shared<PartConfig>())->Load(part.second);

	Model::SetFromJSON(pt);
}

void CellViewer::Update()
{
	// name
	{
		s3d::SimpleGUI::TextBox(m_textEditState_name, s3d::Vec2(10, 10), 240);
		m_model->SetName(s3d::Unicode::Narrow(m_textEditState_name.text));
	}
}
