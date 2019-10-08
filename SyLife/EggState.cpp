#include "EggState.h"

#include "SystemManager.h"
#include "CellManager.h"

#include "CellModel.h"
#include "CellState.h"

void EggState::setCellModel(const shared_ptr<CellModel>& cellModel)
{
	m_cellModel = cellModel;
	SetRadius(cellModel->getRadius() / 2.0);
	SetMass(cellModel->getMass() / 4.0);
	SetInertia(getMass() * getRadius() * getRadius() / 2.0);
}

void EggState::updateEgg()
{
	m_timer -= g_systemManagerPtr->GetDeltaTime();

	// 孵化
	if (m_timer < 0 && RandomBool(0.1))
	{
		Destroy();

		const auto& c = g_cellManagerPtr->AddCellState(getCellModel());
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

	m_cellModel->draw(min(2.0, 10.0 - m_timer) * 0.25);
}