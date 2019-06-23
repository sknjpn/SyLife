#pragma once

#include "Equipment.h"
#include "Viewer.h"

class EyeViewer;
class EyeConfig;

class EyeModel
	: public EquipmentModel
{
public:
	void	MakeViewers() override;
	shared_ptr<PartConfig>	MakePartConfig() const override;
};

class EyeConfig
	: public EquipmentConfig
{
public:
};

class EyeViewer
	: public EquipmentViewer
{
public:
	EyeViewer(const shared_ptr<PartModel>& model)
		: EquipmentViewer(model)
	{}
};

