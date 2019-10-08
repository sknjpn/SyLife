#pragma once


class ElementState
	: public Rigidbody
{
	shared_ptr<ElementModel>	m_model;

public:
	// Get
	const shared_ptr<ElementModel>& GetModel() const { return m_model; }

	// Set
	void	SetModel(const shared_ptr<ElementModel>& model)
	{
		m_model = model;
		SetRadius(model->GetRadius());
		SetMass(model->GetMass());
	}

	void	UpdateElement();
	void	Draw();
};