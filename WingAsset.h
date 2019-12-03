#pragma once

#include "PartAsset.h"

class WingAsset
	: public PartAsset
{
	bool	m_isRight = false;

public:
	class Editor
		: public AssetEditor
	{
		shared_ptr<WingAsset> m_wingAsset;

	public:
		Editor(const shared_ptr<WingAsset>& wingAsset)
			: m_wingAsset(wingAsset)
		{}

		void	init() override;
		void	update() override;
	};

public:
	void	makeEditor(const shared_ptr<Viewer>& parent) { parent->addChildViewer<Editor>(dynamic_pointer_cast<WingAsset>(shared_from_this())); }

	shared_ptr<PartState>	makeState() override;

	// Get
	bool	getIsRight() const { return m_isRight; }

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	
	// TypeName
	String	getTypeName() override { return U"WingAsset"; }
};
