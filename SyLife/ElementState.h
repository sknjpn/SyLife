#pragma once

#include "Rigidbody.h"

class ElementModel;

class ElementState
	: public Rigidbody
{
	shared_ptr<ElementModel>	m_model;

public:
	// Get
	const shared_ptr<ElementModel>& getModel() const { return m_model; }

	// Set
	void	setModel(const shared_ptr<ElementModel>& model);

	void	updateElement();
	void	draw();
};