#pragma once

#include "Part.h"

class BodyModel
	: public PartModel
{
public:
	void	MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const
	{
		PartModel::save_this(pt);

		// type
		pt.put("type", "BodyModel");
	}
	void	save(ptree& pt) const override { save_this(pt); }
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
	// Reload
	void reloadProperties_this()
	{
		PartViewer::reloadProperties_this();
	}
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void Update_this()
	{
		PartViewer::Update_this();
	}
	void Update() override { Update_this(); }
};

inline void		BodyModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<BodyViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}
inline shared_ptr<PartState>	BodyModel::MakeState() { return make_shared<BodyState>(); }

inline void BodyModel::load_this(const ptree& pt)
{
	PartModel::load_this(pt);
}
