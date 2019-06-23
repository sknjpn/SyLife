#include "Module.h"

#include "ViewerManager.h"
#include "AssetManager.h"

void ModuleModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<ModuleViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

ptree ModuleModel::AddToJSON(ptree pt) const
{
	// type
	pt.put("type", "Module");

	return PartModel::AddToJSON(pt);
}

void ModuleModel::SetFromJSON(const ptree & pt)
{
	PartModel::SetFromJSON(pt);
}

ptree ModuleConfig::AddToJSON(ptree pt) const
{
	// type
	pt.put("type", "Module");

	return PartConfig::AddToJSON(pt);
}

void ModuleConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetModel<ModuleModel>(pt.get<string>("name"));

	PartConfig::SetFromJSON(pt);
}

void ModuleViewer::Update()
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
