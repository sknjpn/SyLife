#include "ElementState.h"

void ElementState::setModel(const shared_ptr<ElementModel>& model)
{
	m_model = model;
	setRadius(model->GetRadius());
	setMass(model->GetMass());
}

void ElementState::updateElement()
{
	/*
	// Amino Acidの分解
	if (m_model->GetName() == "Amino Acid" && RandomBool(0.1))
	{
		g_elementManagerPtr->addElementState(g_assetManagerPtr->getModel<ElementModel>("Nitrogen"), GetPosition() + Vec2(20.0, 0.0).rotated(rand() / 360.0));
		g_elementManagerPtr->addElementState(g_assetManagerPtr->getModel<ElementModel>("Carbon"), GetPosition() + Vec2(20.0, 0.0).rotated(rand() / 360.0));
		g_elementManagerPtr->addElementState(g_assetManagerPtr->getModel<ElementModel>("Oxygen"), GetPosition() + Vec2(20.0, 0.0).rotated(rand() / 360.0));

		destroy();
		return;
	}
	*/

	// if (RandomBool(0.01)) destroy();
}

void ElementState::draw()
{
	static Texture particle(U"assets/image/particle.png", TextureDesc::Mipped);

	particle.resized(m_model->GetRadius() * 2.0, m_model->GetRadius() * 2.0).drawAt(GetPosition(), m_model->GetColor());
	/*
	Circle(GetPosition().x, GetPosition().y, GetRadius())
		.draw(ColorF(m_model->GetColor(), 0.5));
		//.drawFrame(1.0, ColorF(1.0, 0.5));
		*/
}