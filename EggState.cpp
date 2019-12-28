#include "EggState.h"
#include "CellAsset.h"
#include "CellState.h"
#include "World.h"

EggState::EggState(const shared_ptr<CellAsset>& cellAsset)
	: m_cellAsset(cellAsset)
	, m_timer(m_cellAsset->getBornTime())
{
	setRadius(cellAsset->getRadius() / 2.0);
	setMass(cellAsset->getMass() / 4.0);
	setInertia(getMass() * getRadius() * getRadius() / 2.0);
}

void EggState::updateEgg()
{
	m_timer -= DeltaTime;

	// 孵化
	if (m_timer <= 0)
	{
		destroy();

		const auto& c = World::GetInstance()->addCellState(getCellAsset());
		c->setPosition(getPosition());
		c->setRotation(getRotation());
	}
}

void EggState::draw()
{
	auto t1 = Transformer2D(getMat3x2());
	auto t2 = Transformer2D(Mat3x2::Scale(0.5));

	Circle(getRadius() * 2.0)
		.draw(ColorF(Palette::Papayawhip, 0.5))
		.drawFrame(1.0, ColorF(1.0, 0.5));

	m_cellAsset->draw(min(2.0, 10.0 - m_timer) * 0.25);
}

void EggState::load(Deserializer<ByteArray>& reader)
{
	Rigidbody::load(reader);

	{
		String cellAssetName;
		reader >> cellAssetName;
		m_cellAsset = World::GetAsset<CellAsset>(cellAssetName);
	}
}

void EggState::save(Serializer<MemoryWriter>& writer) const
{
	Rigidbody::save(writer);

	writer << m_cellAsset->getName();
}
