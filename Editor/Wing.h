#pragma once

#include "Equipment.h"

class WingViewer;
class WingConfig;

class WingModel
	: public EquipmentModel
{
public:
	shared_ptr<PartConfig>	MakePartConfig() const override;
	void	MakeViewers() override;
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

inline shared_ptr<PartConfig> WingModel::MakePartConfig() const { return make_shared<WingConfig>(); }
