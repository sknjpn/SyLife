#pragma once

#include "PartAsset.h"

class PartAsset_Wing
	: public PartAsset
{
	bool	m_isRight;
	int		m_penetrating;

public:
	shared_ptr<PartState>	makePartState(const shared_ptr<PartConfig>& partConfig) const override;

	// Get
	bool	getIsRight() const { return m_isRight; }
	double	getPenetrating() const { return m_penetrating; }

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	
	// TypeName
	String	getTypeName() override { return U"PartAsset_Wing"; }
};
