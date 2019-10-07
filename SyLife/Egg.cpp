#include "EggState.h"

#include "SystemManager.h"
#include "CellManager.h"

void EggState::UpdateEgg()
{
	m_timer -= g_systemManagerPtr->GetDeltaTime();

	// 孵化
	if (m_timer < 0 && RandomBool(0.1))
	{
		Destroy();

		const auto& c = g_cellManagerPtr->AddCellState(GetCellModel());
		c->setPosition(getPosition());
		c->setRotation(getRotation());
	}
}

void EggState::Draw()
{
	auto t = Transformer2D(Mat3x2::Scale(0.5)
		.rotated(getRotation())
		.translated(getPosition().x, getPosition().y));

	Circle(getRadius() * 2.0)
		.draw(ColorF(Palette::Papayawhip, 0.5))
		.drawFrame(1.0, ColorF(1.0, 0.5));

	m_cellModel->Draw(min(2.0, 10.0 - m_timer) * 0.25);
}