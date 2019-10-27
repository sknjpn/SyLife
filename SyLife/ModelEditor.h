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
	shared_ptr<T>	getPartAsset() const { return dynamic_pointer_cast<T>(m_model); }

	// Set
	void	setPartAsset(const shared_ptr<Model> model) { m_model = model; reloadProperties(); }

	// Reload
	void	reloadProperties_this();
	virtual void reloadProperties() { reloadProperties_this(); }

	// update
	void	update_this();
	void	update() override { update_this(); }
};
