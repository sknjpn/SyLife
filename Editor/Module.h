#pragma once

#include "Part.h"
#include "Viewer.h"

#include "ViewerManager.h"
#include "AssetManager.h"

class ModuleViewer;
class ModuleConfig;

class ModuleModel
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

class ModuleConfig
	: public PartConfig
{
public:
	shared_ptr<ModuleModel>	m_model;

public:
	ptree	AddToJSON(ptree pt) const;
	void	SetFromJSON(const ptree& pt);

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	Save() const override { return AddToJSON(ptree()); }
};

class ModuleViewer
	: public PartViewer
{
public:
	s3d::TextEditState		m_textEditState_name;
	s3d::TextEditState		m_textEditState_mass;

public:
	ModuleViewer(const shared_ptr<PartModel>& model)
		: PartViewer(model)
		, m_textEditState_name(s3d::Unicode::Widen(model->m_name))
		, m_textEditState_mass(s3d::ToString(model->m_mass))
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}

	void	Update() override;
};

inline shared_ptr<PartConfig> ModuleModel::MakeConfig() const { return make_shared<ModuleConfig>(); }
