#pragma once

#include "Part.h"

class BodyModel
	: public PartModel
{
public:
	shared_ptr<Viewer>		MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const
	{
		PartModel::AddToJSON(pt);

		// type
		pt.put("type", "BodyModel");
	}
	void	Save(ptree& pt) const override { AddToJSON(pt); }
};

class BodyState
	: public PartState
{
public:
	void	Draw(const CellState& cell) const { GetPartConfig()->GetModel()->Draw(); }
	void	Update(CellState& cell) override {}
};

class BodyViewer
	: public PartViewer
{
public:
	BodyViewer(const shared_ptr<PartModel>& model)
		: PartViewer(model)
	{
		SetDrawRect(0, 450, 600, 150);
	}

	void Update() override
	{
	}
};

inline shared_ptr<Viewer>		BodyModel::MakeViewer() { return g_viewerManagerPtr->MakeViewer<BodyViewer>(dynamic_pointer_cast<PartModel>(shared_from_this())); }
inline shared_ptr<PartState>	BodyModel::MakeState() { return make_shared<BodyState>(); }

inline void BodyModel::SetFromJSON(const ptree& pt)
{
	PartModel::SetFromJSON(pt);
}
