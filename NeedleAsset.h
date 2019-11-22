#pragma once

#include "PartAsset.h"

class NeedleAsset
	: public PartAsset
{
public:
	class Editor
		: public AssetEditor
	{
		shared_ptr<NeedleAsset>	m_needleAsset;

	public:
		Editor(const shared_ptr<NeedleAsset>& needleAsset)
			: m_needleAsset(needleAsset)
		{}

		void	init() override;
		void	update() override;
	};

public:
	// Editor
	void	makeEditor(const shared_ptr<Viewer>& parent) { parent->addChildViewer<Editor>(dynamic_pointer_cast<NeedleAsset>(shared_from_this())); }

	// State
	shared_ptr<PartState>	makeState() override;

	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
};
