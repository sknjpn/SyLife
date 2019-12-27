#include "ProteinState.h"
#include "ProteinAsset.h"
#include "TileState.h"
#include "World.h"

ProteinState::ProteinState(const shared_ptr<ProteinAsset>& asset)
	: m_proteinAsset(asset)
{
	setRadius(asset->getRadius());
}

void ProteinState::updateProtein()
{
	// 分解
	if (RandomBool(0.01))
	{
		World::GetInstance()->getTile(getPosition())->addNutrition(m_proteinAsset->getMaterial().getNutritionRecursive());

		destroy();

		return;
	}
}

void ProteinState::draw()
{
	static Texture particle(U"resources/image/particle.png", TextureDesc::Mipped);

	//particle.resized(m_proteinAsset->getRadius() * 2.0, m_proteinAsset->getRadius() * 2.0).drawAt(getPosition(), m_proteinAsset->getColor());

	Circle(getPosition(), getRadius())
		.draw(ColorF(m_proteinAsset->getColor(), 1.0));
}

void ProteinState::load(Deserializer<ByteArray>& reader)
{
	Rigidbody::load(reader);

	{
		String proteinAssetName;
		reader >> proteinAssetName;
		m_proteinAsset = World::GetAsset<ProteinAsset>(proteinAssetName);
	}

	setRadius(m_proteinAsset->getRadius());
}

void ProteinState::save(Serializer<MemoryWriter>& writer) const
{
	Rigidbody::save(writer);

	writer << m_proteinAsset->getName();
}
