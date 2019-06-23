#include "Equipment.h"

#include "ViewerManager.h"
#include "AssetManager.h"

void EquipmentModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<EquipmentViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

ptree EquipmentModel::AddToJSON(ptree pt) const
{
	// type
	pt.put("type", "Equipment");

	return PartModel::AddToJSON(pt);
}

void EquipmentModel::SetFromJSON(const ptree & pt)
{
	PartModel::SetFromJSON(pt);
}


ptree EquipmentConfig::AddToJSON(ptree pt) const
{
	// type
	pt.put("type", "Equipment");

	return PartConfig::AddToJSON(pt);
}

void EquipmentConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetModel<EquipmentModel>(pt.get<string>("name"));

	PartConfig::SetFromJSON(pt);
}

void EquipmentViewer::Update()
{
	// name
	{
		s3d::SimpleGUI::TextBox(m_textEditState_name, s3d::Vec2(10, 10), 240);
		m_model->m_name = s3d::Unicode::Narrow(m_textEditState_name.text);
	}

	// mass
	{
		s3d::SimpleGUI::TextBox(m_textEditState_mass, s3d::Vec2(10, 50), 240);
		m_model->m_mass = s3d::Parse<double>(m_textEditState_mass.text);
	}
}
