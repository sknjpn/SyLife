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
	shared_ptr<T>	GetModel() const { return dynamic_pointer_cast<T>(m_model); }

	// Set
	void	SetModel(const shared_ptr<Model> model) { m_model = model; ReloadProperties(); }

	// Reload
	void ReloadProperties_this() 
	{
		// name
		m_textEditState_name.text = Unicode::Widen(GetModel<Model>()->GetName());
	}
	virtual void ReloadProperties() { ReloadProperties_this(); }

	// Update
	void	Update_this()
	{
		// name
		SimpleGUI::TextBox(m_textEditState_name, Vec2(8, 4), 240);
		GetModel<Model>()->SetName(Unicode::Narrow(m_textEditState_name.text));
		MoveDrawPos(0, 48);
	}
	void	Update() override { Update_this(); }
};
