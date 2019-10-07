#pragma once

#include "Viewer.h"
#include "Model.h"

class ModelViewer
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
	void reloadProperties_this() 
	{
		// name
		m_textEditState_name.text = Unicode::Widen(getModel<Model>()->getName());
	}
	virtual void reloadProperties() { reloadProperties_this(); }

	// Update
	void	Update_this()
	{
		// name
		SimpleGUI::TextBox(m_textEditState_name, Vec2(8, 8), 240);
		getModel<Model>()->setName(Unicode::Narrow(m_textEditState_name.text));
		MoveDrawPos(0, 48);
	}
	void	Update() override { Update_this(); }
};
