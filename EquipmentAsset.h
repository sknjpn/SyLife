#pragma once

#include "PartAsset.h"

class EquipmentAsset
	: public PartAsset
{
public:
	void makeViewer() override;
	shared_ptr<PartState>	makeState() override;

	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
};
