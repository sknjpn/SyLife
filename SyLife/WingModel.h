#pragma once

#include "EquipmentModel.h"
#include "Cell.h"

class WingModel
	: public EquipmentModel
{
public:
	bool	m_isRight = false;

public:
	void makeViewer() override;
	shared_ptr<PartState>	MakeState() override;


	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const
	{
		pt.put("isRight", m_isRight);

		EquipmentModel::save_this(pt);

		pt.put("type", "WingModel");
	}
	void	save(ptree& pt) const override { save_this(pt); }
};
