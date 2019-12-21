#pragma once

#include "PartAsset.h"

class PartAsset_Needle
	: public PartAsset
{
public:
	class Editor
		: public AssetEditor
	{
		shared_ptr<PartAsset_Needle>	m_needleAsset;

	public:
		Editor(const shared_ptr<PartAsset_Needle>& needleAsset)
			: m_needleAsset(needleAsset)
		{}

		void	init() override;
		void	update() override;
	};

public:
	// Editor
	void	makeEditor(const shared_ptr<Viewer>& parent) { parent->addChildViewer<Editor>(dynamic_pointer_cast<PartAsset_Needle>(shared_from_this())); }

	// State
	shared_ptr<PartState>	makeState() override;

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	
	// TypeName
	String	getTypeName() override { return U"PartAsset_Needle"; }
};
