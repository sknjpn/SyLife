﻿#pragma once

#include "PartAsset.h"

class NucleusAsset
	: public PartAsset
{
	double	m_lifespanTime;	// 寿命
	double	m_bornTime;		// 生まれるまでの時間
	double	m_yieldTime;	// 生むのにかかる時間

public:
	class Editor
		: public AssetEditor
	{
		shared_ptr<NucleusAsset>	m_nucleusAsset;

	public:
		Editor(const shared_ptr<NucleusAsset>& nucleusAsset)
			: m_nucleusAsset(nucleusAsset)
		{}

		void	init() override;
		void	update() override;
	};

public:
	// Editor
	void	makeEditor(const shared_ptr<Viewer>& parent) { parent->addChildViewer<Editor>(dynamic_pointer_cast<NucleusAsset>(shared_from_this())); }

	// State
	shared_ptr<PartState>	makeState() override;

	// Get
	double	getLifespanTime() const { return m_lifespanTime; }
	double	getBornTime() const { return m_bornTime; }
	double	getYieldTime() const { return m_yieldTime; }

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	
	// TypeName
	String	getTypeName() override { return U"NucleusAsset"; }
};
