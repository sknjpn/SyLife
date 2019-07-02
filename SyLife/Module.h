#pragma once

#include "Part.h"
#include "ViewerManager.h"

class ModuleModel
	: public PartModel
{
public:
	shared_ptr<Viewer> MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const
	{
		PartModel::AddToJSON(pt);

		// type
		pt.put("type", "ModuleModel");
	}
	void	Save(ptree& pt) const override { AddToJSON(pt); }
};

class ModuleState
	: public PartState
{
public:
	void	Draw(const CellState& cell) const { GetPartConfig()->GetModel()->Draw(); }
	void	Update(CellState& cell) override {}
};

class ModuleViewer
	: public PartViewer
{
public:
	TextEditState		m_textEditState_name;
	TextEditState		m_textEditState_mass;

public:
	ModuleViewer(const shared_ptr<PartModel>& model)
		: PartViewer(model)
		, m_textEditState_name(Unicode::Widen(model->GetName()))
		, m_textEditState_mass(ToString(model->GetMass()))
	{
		m_drawRect = RectF(0, 450, 600, 150);
	}

	void Update(bool isMouseOver) override {}
};

inline shared_ptr<Viewer> ModuleModel::MakeViewer() { return g_viewerManagerPtr->MakeViewer<ModuleViewer>(dynamic_pointer_cast<PartModel>(shared_from_this())); }


inline shared_ptr<PartState>	ModuleModel::MakeState() { return make_shared<ModuleState>(); }

inline void ModuleModel::SetFromJSON(const ptree& pt)
{
	PartModel::SetFromJSON(pt);
}
