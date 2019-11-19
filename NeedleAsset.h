#pragma once

#include "EquipmentAsset.h"

class NeedleAsset
	: public EquipmentAsset
{
public:
	void	makeViewer() override;
	shared_ptr<PartState>	makeState() override;

	void	load(const JSONValue& json) override;
	void	save(const JSONWriter& json) const override;
};
