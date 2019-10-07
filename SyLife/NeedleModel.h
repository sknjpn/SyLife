#pragma once

#include "EquipmentModel.h"
#include "Cell.h"

class NeedleModel
	: public EquipmentModel
{
public:
	void	makeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	load_this(const ptree& pt) { EquipmentModel::load_this(pt); }
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const
	{
		EquipmentModel::save_this(pt);

		// type
		pt.put("type", "NeedleModel");
	}
	void	save(ptree& pt) const override { save_this(pt); }
};
