#pragma once

#include "Rigidbody.h"

class ElementAsset;

class ElementState
	: public Rigidbody
{
	shared_ptr<ElementAsset>	m_elementAsset;

public:
	ElementState(const shared_ptr<ElementAsset>& asset);
	ElementState(const JSONValue& json) { load(json); }

	// Get
	const shared_ptr<ElementAsset>& getPartAsset() const { return m_elementAsset; }

	void	updateElement();
	void	draw();

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
};