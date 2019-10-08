#include "ElementState.h"

#include "ElementAsset.h"

void ElementState::setElementAsset(const shared_ptr<ElementAsset>& elementAsset)
{
	m_elementAsset = elementAsset;
	setRadius(elementAsset->getRadius());
	setMass(elementAsset->getMass());
}

void ElementState::updateElement()
{
	/*
	// Amino Acidの分解
	if (m_elementAsset->GetName() == "Amino Acid" && RandomBool(0.1))
	{
		g_elementManagerPtr->addElementState(g_assetManagerPtr->getelementAsset<ElementelementAsset>("Nitrogen"), GetPosition() + Vec2(20.0, 0.0).rotated(rand() / 360.0));
		g_elementManagerPtr->addElementState(g_assetManagerPtr->getelementAsset<ElementelementAsset>("Carbon"), GetPosition() + Vec2(20.0, 0.0).rotated(rand() / 360.0));
		g_elementManagerPtr->addElementState(g_assetManagerPtr->getelementAsset<ElementelementAsset>("Oxygen"), GetPosition() + Vec2(20.0, 0.0).rotated(rand() / 360.0));

		destroy();
		return;
	}
	*/

	// if (RandomBool(0.01)) destroy();
}

void ElementState::draw()
{
	static Texture particle(U"assets/image/particle.png", TextureDesc::Mipped);

	particle.resized(m_elementAsset->getRadius() * 2.0, m_elementAsset->getRadius() * 2.0).drawAt(getPosition(), m_elementAsset->GetColor());
	/*
	Circle(GetPosition().x, GetPosition().y, GetRadius())
		.draw(ColorF(m_elementAsset->GetColor(), 0.5));
		//.drawFrame(1.0, ColorF(1.0, 0.5));
		*/
}