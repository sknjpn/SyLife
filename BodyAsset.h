#pragma once

#include "PartAsset.h"

class BodyAsset
	: public PartAsset
{
public:
	void	makeViewer() override;
	shared_ptr<PartState>	makeState() override;

	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
};
