#include "EggState.h"

#include "SystemManager.h"
#include "CellManager.h"

#include "CellAsset.h"
#include "CellState.h"

void EggState::setCellAsset(const shared_ptr<CellAsset>& cellModel)
{
	m_cellAsset = cellModel;
	setRadius(cellModel->getRadius() / 2.0);
	setMass(cellModel->getMass() / 4.0);
	setInertia(getMass() * getRadius() * getRadius() / 2.0);
}

void EggState::updateEgg()
{
	m_timer -= g_systemManagerPtr->GetDeltaTime();

	// 孵化
	if (m_timer < 0 && RandomBool(0.1))
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