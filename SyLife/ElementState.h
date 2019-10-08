#pragma once

#include "Rigidbody.h"

class ElementModel;

class ElementState
	: public Rigidbody
{
	shared_ptr<ElementModel>	m_model;

public:
	// Get
	const shared_ptr<ElementModel>& GetModel() const { return m_model; }

	// Set
	void	SetModel(const shared_ptr<ElementModel>& model);

	void	UpdateElement();
	void	Draw();
};