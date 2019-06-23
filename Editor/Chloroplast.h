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

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	AddToJSON(ptree pt) const;
	ptree	Save() const override { return AddToJSON(ptree()); }
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
	g_viewerManagerPtr->AddViewer<ChloroplastViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

inline void ChloroplastModel::SetFromJSON(const ptree & pt)
{
	ModuleModel::SetFromJSON(pt);
}

inline ptree ChloroplastModel::AddToJSON(ptree pt) const
{
	// type
	pt.put("type", "ChloroplastModel");


	return ModuleModel::AddToJSON(pt);
}
