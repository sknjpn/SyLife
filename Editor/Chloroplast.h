#pragma once

#include "Module.h"
#include "Viewer.h"

#include "ViewerManager.h"
#include "AssetManager.h"

class ChloroplastViewer;

class ChloroplastModel
	: public ModuleModel
{
public:
	void	MakeViewers() override;

	void	Load_this(const ptree& pt);
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const;
	void	Save(ptree& pt) const override { Save_this(pt); }
};

class ChloroplastViewer
	: public ModuleViewer
{
public:
	ChloroplastViewer(const shared_ptr<PartModel>& model)
		: ModuleViewer(model)
	{}
};

inline void ChloroplastModel::MakeViewers()
{
	g_viewerManagerPtr->MakeViewer<ChloroplastViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

inline void ChloroplastModel::Load_this(const ptree& pt)
{
	ModuleModel::Load_this(pt);
}

inline void ChloroplastModel::Save_this(ptree& pt) const
{
	ModuleModel::Save_this(pt);

	// type
	pt.put("type", "ChloroplastModel");
}
