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
	void	load(const JSONValue& json) override;
	void	save(ptree& pt) const override;
};
