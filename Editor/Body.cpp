#include "Body.h"

#include "ViewerManager.h"
#include "AssetManager.h"

void BodyModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<BodyViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

ptree BodyModel::AddToJSON(ptree pt) const
{
	// type
	pt.put("type", "Body");

	return PartModel::AddToJSON(pt);
}

void BodyModel::SetFromJSON(const ptree & pt)
{
	PartModel::SetFromJSON(pt);
}

ptree BodyConfig::AddToJSON(ptree pt) const
{
	// type
	pt.put("type", "Body");

	return PartConfig::AddToJSON(pt);
}

void BodyConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetModel<BodyModel>(pt.get<string>("name"));

	PartConfig::SetFromJSON(pt);
}


void BodyViewer::Update()
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
