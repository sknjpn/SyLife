#include "Egg.h"
#include "FieldManager.h"
#include "Random.h"

void EggState::UpdateEgg()
{
	m_timer -= g_fieldManagerPtr->GetDeltaTime();

	// ›z‰»
	if (m_timer < 0 && RandomBool(0.1))
	{
		Destroy();

		const auto& c = g_cellManagerPtr->AddCellState(GetCellModel());
		c->SetPosition(GetPosition());
		c->SetRotation(GetRotation());
	}
}

void EggState::Draw()
{
	auto t = s3d::Transformer2D(s3d::Mat3x2::Scale(0.5)
		.rotated(GetRotation())
		.translated(GetPosition().m_x, GetPosition().m_y));

	s3d::Circle(GetRadius() * 2.0)
		.draw(s3d::ColorF(s3d::Palette::Papayawhip, 0.5))
		.drawFrame(1.0, s3d::ColorF(1.0, 0.5));

	m_cellModel->Draw(min(2.0, 10.0 - m_timer) * 0.25);
}