#pragma once

#include "Equipment.h"

#include "ViewerManager.h"
#include "AssetManager.h"

class WingViewer;

class WingModel
	: public EquipmentModel
{
public:
	void	MakeViewers() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const;
	void	Save(ptree& pt) const override { AddToJSON(pt); }
};

class WingViewer
	: public EquipmentViewer
{
public:
	WingViewer(const shared_ptr<PartModel>& model)
		: EquipmentViewer(model)
	{}
};

inline void WingModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<WingViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

inline void WingModel::SetFromJSON(const ptree& pt)
{
	EquipmentModel::SetFromJSON(pt);
}

inline void WingModel::AddToJSON(ptree& pt) const
{
	EquipmentModel::AddToJSON(pt);

	pt.put("type", "WingModel");
}