#include "EggState.h"

#include "SystemManager.h"
#include "CellManager.h"

#include "CellAsset.h"
#include "CellState.h"

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
	m_timer -= g_systemManagerPtr->GetDeltaTime();

	// 孵化
	if (m_timer <= 0)
	{
		destroy();

		const auto& c = g_cellManagerPtr->addCellState(getCellAsset());
		c->setPosition(getPosition());
		c->setRotation(getRotation());
	}
}

void EggState::draw()
{
	auto t = Transformer2D(Mat3x2::Scale(0.5)
		.rotated(getRotation())
		.translated(getPosition().x, getPosition().y));

	Circle(getRadius() * 2.0)
		.draw(ColorF(Palette::Papayawhip, 0.5))
		.drawFrame(1.0, ColorF(1.0, 0.5));

	m_cellAsset->draw(min(2.0, 10.0 - m_timer) * 0.25);
}