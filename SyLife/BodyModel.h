﻿#pragma once

#include "PartModel.h"

class BodyModel
	: public PartModel
{
public:
	void	makeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	load_this(const ptree& pt){ PartModel::load_this(pt); }
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const
	{
		PartModel::save_this(pt);

		// type
		pt.put("type", "BodyModel");
	}
	void	save(ptree& pt) const override { save_this(pt); }
};