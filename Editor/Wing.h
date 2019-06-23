#pragma once

#include "Equipment.h"

#include "ViewerManager.h"
#include "AssetManager.h"

class WingViewer;
class WingConfig;

class WingModel
	: public EquipmentModel
{
public:
	shared_ptr<PartConfig>	MakeConfig() const override;
	void	MakeViewers() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	AddToJSON(ptree pt) const;
	ptree	Save() const override { return AddToJSON(ptree()); }
};

class WingConfig
	: public EquipmentConfig
{
public:

};

class WingViewer
	: public EquipmentViewer
{
public:
	WingViewer(const shared_ptr<PartModel>& model)
		: EquipmentViewer(model)
	{}
};

inline shared_ptr<PartConfig> WingModel::MakeConfig() const { return make_shared<WingConfig>(); }

inline void WingModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<WingViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

inline ptree WingModel::AddToJSON(ptree pt) const
{
	return ptree();
}

inline void WingModel::SetFromJSON(const ptree & pt)
{
}