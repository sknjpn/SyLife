#pragma once

#include "EquipmentAsset.h"

class NeedleAsset
	: public EquipmentAsset
{
public:
	void	makeViewer() override;
	shared_ptr<PartState>	makeState() override;

	void	load(const ptree& pt) override;
	void	save(ptree& pt) const override;
};
