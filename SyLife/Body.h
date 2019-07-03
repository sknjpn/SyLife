#pragma once

#include "Part.h"

class BodyModel
	: public PartModel
{
public:
	void		MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	Load_this(const ptree& pt);
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const
	{
		PartModel::Save_this(pt);

		// type
		pt.put("type", "BodyModel");
	}
	void	Save(ptree& pt) const override { Save_this(pt); }
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
	BodyViewer()
	{
		SetDrawRect(0, 450, 600, 150);
	}

	void Update() override
	{
	}
};

inline void		BodyModel::MakeViewer() 
{
	g_viewerManagerPtr->MakeViewer<BodyViewer>()->SetModel(shared_from_this()); 
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}
inline shared_ptr<PartState>	BodyModel::MakeState() { return make_shared<BodyState>(); }

inline void BodyModel::Load_this(const ptree& pt)
{
	PartModel::Load_this(pt);
}
