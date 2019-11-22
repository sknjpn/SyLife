#pragma once

#include "PartAsset.h"

class BodyAsset
	: public PartAsset
{
public:
	class Editor
		: public AssetEditor
	{
		shared_ptr<BodyAsset>	m_bodyAsset;

	public:
		Editor(const shared_ptr<BodyAsset>& bodyAsset)
			: m_bodyAsset(bodyAsset)
		{}

		void	init() override;
		void	update() override;
	};

public:
	// Editor
	void	makeEditor(const shared_ptr<Viewer>& parent) { parent->addChildViewer<Editor>(dynamic_pointer_cast<BodyAsset>(shared_from_this())); }

	// State
	shared_ptr<PartState>	makeState() override;

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
};
