#include "ElementState.h"

#include "ChipManager.h"
#include "ElementManager.h"

#include "ElementAsset.h"

void ElementState::setElementAsset(const shared_ptr<ElementAsset>& elementAsset)
{
	m_elementAsset = elementAsset;
	setRadius(elementAsset->getRadius());
	setMass(elementAsset->getMass());
}

void ElementState::updateElement()
{
	// 分解
	if (RandomBool(0.01))
	{
		g_chipManagerPtr->addNutrition(getPosition(), m_elementAsset->getMaterial().getNutrition());

		for(const auto& e : m_elementAsset->getMaterial().getElementList())
			g_elementManagerPtr->addElementState(e.first, getPosition() + Vec2(20.0, 0.0).rotated(rand() / 360.0));

		destroy();
		return;
	}

	// if (RandomBool(0.01)) destroy();
}

void ElementState::draw()
{
	static Texture particle(U"assets/image/particle.png", TextureDesc::Mipped);

	particle.resized(m_elementAsset->getRadius() * 2.0, m_elementAsset->getRadius() * 2.0).drawAt(getPosition(), m_elementAsset->getColor());
	/*
	Circle(GetPosition().x, GetPosition().y, GetRadius())
		.draw(ColorF(m_elementAsset->GetColor(), 0.5));
		//.drawFrame(1.0, ColorF(1.0, 0.5));
		*/
}