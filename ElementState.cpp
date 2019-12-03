#include "ElementState.h"
#include "ElementAsset.h"
#include "World.h"

ElementState::ElementState(const shared_ptr<ElementAsset>& asset)
	: m_elementAsset(asset)
{
	setRadius(asset->getRadius());
}

void ElementState::updateElement()
{
	// 分解
	if (RandomBool(0.01))
	{
		World::GetInstance()->getField().getChip(getPosition())->addNutrition(m_elementAsset->getMaterial().getNutritionRecursive());

		destroy();

		return;
	}
}

void ElementState::draw()
{
	static Texture particle(U"resources/image/particle.png", TextureDesc::Mipped);

	//particle.resized(m_elementAsset->getRadius() * 2.0, m_elementAsset->getRadius() * 2.0).drawAt(getPosition(), m_elementAsset->getColor());

	Circle(getPosition(), getRadius())
		.draw(ColorF(m_elementAsset->getColor(), 1.0));
}

void ElementState::load(const JSONValue& json)
{
	Rigidbody::load(json);

	const auto assetName = json[U"elementAsset"].getString();
	m_elementAsset = World::GetInstance()->getAssets().getAsset<ElementAsset>(assetName);
	
	setRadius(m_elementAsset->getRadius());
}

void ElementState::save(JSONWriter& json) const
{
	Rigidbody::save(json);

	json.key(U"elementAsset").write(m_elementAsset->getName());
}
