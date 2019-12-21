#pragma once

#include "PartAsset.h"

class PartAsset_Synthesizer
	: public PartAsset
{
	shared_ptr<ElementAsset>	m_export;

public:
	class Editor
		: public AssetEditor
	{
		shared_ptr<PartAsset_Synthesizer>	m_synthesizerAsset;
		
	public:
		Editor(const shared_ptr<PartAsset_Synthesizer>& synthesizerAsset)
			: m_synthesizerAsset(synthesizerAsset)
		{}

		void	init() override;
		void	update() override;
	};

public:
	// Editor
	void	makeEditor(const shared_ptr<Viewer>& parent) { parent->addChildViewer<Editor>(dynamic_pointer_cast<PartAsset_Synthesizer>(shared_from_this())); }

	// State
	shared_ptr<PartState>	makeState() override;

	// Get
	const shared_ptr<ElementAsset>& getExport() const { return m_export; }

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	
	// TypeName
	String	getTypeName() override { return U"PartAsset_Synthesizer"; }
};
