#pragma once

#include "Equipment.h"

class WingViewer;
class WingConfig;

class WingModel
	: public EquipmentModel
{
public:
	void	MakeViewers() override;
	shared_ptr<PartConfig>	MakePartConfig() const override;
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
