#pragma once

#include "EquipmentAsset.h"

class WingAsset
	: public EquipmentAsset
{
public:
	bool	m_isRight = false;

public:
	void makeViewer() override;
	shared_ptr<PartState>	makeState() override;

	// JSON
	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
};
