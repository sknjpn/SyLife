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

	void	Load_this(const ptree& pt);
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const;
	void	Save(ptree& pt) const override { Save_this(pt); }
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

inline void EyeModel::Load_this(const ptree& pt)
{
	EquipmentModel::Load_this(pt);
}

inline void EyeModel::Save_this(ptree& pt) const
{
	EquipmentModel::Save_this(pt);

	// type
	pt.put("type", "EyeModel");
}
