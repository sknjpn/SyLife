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

	void	load(const ptree& pt) override;
	void	save(ptree& pt) const override;
};
