#pragma once

#include "PartAsset.h"

class PartAsset_Nucleus
	: public PartAsset
{
	double	m_lifespanTime;	// 寿命
	double	m_bornTime;		// 生まれるまでの時間
	double	m_yieldTime;	// 生むのにかかる時間
	int		m_armor;

public:
	// State
	std::shared_ptr<PartState>	makePartState(const std::shared_ptr<PartConfig>& partConfig) const override;

	// Get
	double	getLifespanTime() const { return m_lifespanTime; }
	double	getBornTime() const { return m_bornTime; }
	double	getYieldTime() const { return m_yieldTime; }
	int		getArmor() const { return m_armor; }

	bool	isPreRenderOnStateEnabled() const override { return true; }
	bool	isDrawOnStateEnabled() const override { return false; }

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;

	// TypeName
	String	getTypeName() override { return U"PartAsset_Nucleus"; }
};
