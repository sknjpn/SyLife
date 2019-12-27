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

void ElementState::load(Deserializer<ByteArray>& reader)
{
	Rigidbody::load(reader);

	{
		String elementAssetName;
		reader >> elementAssetName;
		m_elementAsset = World::GetAsset<ElementAsset>(elementAssetName);
	}

	setRadius(m_elementAsset->getRadius());
}

void ElementState::save(Serializer<MemoryWriter>& writer) const
{
	Rigidbody::save(writer);

	writer << m_elementAsset->getName();
}
