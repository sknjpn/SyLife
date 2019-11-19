#pragma once

#include "PartAsset.h"

class NucleusAsset
	: public PartAsset
{
	double	m_lifespanTime;	// 寿命
	double	m_bornTime;		// 生まれるまでの時間
	double	m_yieldTime;	// 生むのにかかる時間

public:
	void makeViewer() override;
	shared_ptr<PartState>	makeState() override;

	// Get
	double	getLifespanTime() const { return m_lifespanTime; }
	double	getBornTime() const { return m_bornTime; }
	double	getYieldTime() const { return m_yieldTime; }

	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
};
