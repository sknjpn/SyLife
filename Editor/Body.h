#pragma once

#include "Part.h"
#include "Viewer.h"

#include "ViewerManager.h"
#include "AssetManager.h"

class BodyViewer;
class BodyConfig;

class BodyModel
	: public PartModel
{
public:
	void	MakeViewers() override;
	shared_ptr<PartConfig>	MakeConfig() const override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	AddToJSON(ptree pt) const;
	ptree	Save() const override { return AddToJSON(ptree()); }
};

class BodyConfig
	: public PartConfig
{
public:
	shared_ptr<BodyModel>	m_model;

public:
	ptree	AddToJSON(ptree pt) const;
	void	SetFromJSON(const ptree& pt);

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	Save() const override { return AddToJSON(ptree()); }
};

class BodyViewer
	: public PartViewer
{
public:
	s3d::TextEditState		m_textEditState_name;
	s3d::TextEditState		m_textEditState_mass;

public:
	BodyViewer(const shared_ptr<PartModel>& model)
		: PartViewer(model)
		, m_textEditState_name(s3d::Unicode::Widen(model->m_name))
		, m_textEditState_mass(s3d::ToString(model->m_mass))
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}

	void	Update() override;
};

inline shared_ptr<PartConfig> BodyModel::MakeConfig() const { return make_shared<BodyConfig>(); }


inline void BodyModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<BodyViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

inline ptree BodyModel::AddToJSON(ptree pt) const
{
	// type
	pt.put("type", "Body");

	return PartModel::AddToJSON(pt);
}

inline void BodyModel::SetFromJSON(const ptree & pt)
{
	PartModel::SetFromJSON(pt);
}

inline ptree BodyConfig::AddToJSON(ptree pt) const
{
	// type
	pt.put("type", "Body");

	return PartConfig::AddToJSON(pt);
}

inline void BodyConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetModel<BodyModel>(pt.get<string>("name"));

	PartConfig::SetFromJSON(pt);
}


inline void BodyViewer::Update()
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
