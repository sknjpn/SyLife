#pragma once

#include "Equipment.h"
#include "Viewer.h"

#include "ViewerManager.h"
#include "AssetManager.h"

class EyeViewer;

class EyeModel
	: public EquipmentModel
{
public:
	void	MakeViewers() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const;
	void	Save(ptree& pt) const override { AddToJSON(pt); }
};

class EyeViewer
	: public EquipmentViewer
{
public:
	EyeViewer(const shared_ptr<PartModel>& model)
		: EquipmentViewer(model)
	{}
};

inline void EyeModel::MakeViewers()
{
	g_viewerManagerPtr->MakeViewer<EyeViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

inline void EyeModel::SetFromJSON(const ptree& pt)
{
	EquipmentModel::SetFromJSON(pt);
}

inline void EyeModel::AddToJSON(ptree& pt) const
{
	EquipmentModel::AddToJSON(pt);

	// type
	pt.put("type", "EyeModel");
}
