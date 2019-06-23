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
	shared_ptr<PartConfig>	MakeConfig() const override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	AddToJSON(ptree pt) const;
	ptree	Save() const override { return AddToJSON(ptree()); }
};

class EyeViewer
	: public EquipmentViewer
{
public:
	EyeViewer(const shared_ptr<PartModel>& model)
		: EquipmentViewer(model)
	{}
};

inline shared_ptr<PartConfig> EyeModel::MakeConfig() const { return make_shared<EyeConfig>(); }

inline void EyeModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<EyeViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

inline void EyeModel::SetFromJSON(const ptree & pt)
{
}

inline ptree EyeModel::AddToJSON(ptree pt) const
{
	return ptree();
}
