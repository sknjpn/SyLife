#pragma once

#include "Rigidbody.h"

class ElementAsset;

class ElementState
	: public Rigidbody
{
	shared_ptr<ElementAsset>	m_elementAsset;

public:
	// Get
	const shared_ptr<ElementAsset>& getPartAsset() const { return m_elementAsset; }

	// Set
	void	setElementAsset(const shared_ptr<ElementAsset>& elementAsset);

	void	updateElement();
	void	draw();
};