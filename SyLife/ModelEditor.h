#pragma once

#include "Viewer.h"
#include "Model.h"

class ModelEditor
	: public Viewer
{
	shared_ptr<Model>	m_model;

	TextEditState		m_textEditState_name;

public:
	// Get
	template <typename T>
	shared_ptr<T>	getModel() const { return dynamic_pointer_cast<T>(m_model); }

	// Set
	void	setModel(const shared_ptr<Model> model) { m_model = model; reloadProperties(); }

	// Reload
	void	reloadProperties_this();
	virtual void reloadProperties() { reloadProperties_this(); }

	// update
	void	Update_this();
	void	update() override { Update_this(); }
};
