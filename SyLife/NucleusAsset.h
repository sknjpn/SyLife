#pragma once

#include "PartAsset.h"

class NucleusAsset
	: public PartAsset
{
public:
	void makeViewer() override;
	shared_ptr<PartState>	makeState() override;

	void	load(const ptree& pt) override;
	void	save(ptree& pt) const override;
};
