#pragma once

#include "Viewer.h"
#include "Model.h"

class ModelViewer
	: public Viewer
{
	shared_ptr<Model>	m_model;

public:
	// Get
	template <typename T>
	shared_ptr<T>	GetModel() const { return dynamic_pointer_cast<T>(m_model); }

	// Set
	void	SetModel(const shared_ptr<Model> model) { m_model = model; }
};
