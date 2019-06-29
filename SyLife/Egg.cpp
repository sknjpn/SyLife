#include "Egg.h"
#include "FieldManager.h"
#include "CellManager.h"
#include "Cell.h"
#include "Random.h"

#include <boost/math/constants/constants.hpp>

void EggState::Update()
{
	m_timer -= g_fieldManagerPtr->GetDeltaTime();

	// ›z‰»
	if (m_timer < 0 && RandomBool(0.1))
	{
		m_isDestroyed = true;

		const auto& c = g_cellManagerPtr->AddCellState(GetCellModel());
		c->m_position = m_position;
		c->m_rotation = m_rotation;
	}
}

void EggState::Draw()
{
	auto t = s3d::Transformer2D(s3d::Mat3x2::Scale(0.5).rotated(m_rotation).translated(m_position.m_x, m_position.m_y));

	s3d::Circle(m_radius * 2.0)
		.draw(s3d::ColorF(s3d::Palette::Papayawhip, 0.5))
		.drawFrame(1.0, s3d::ColorF(1.0, 0.5));

	m_cellModel->Draw(min(2.0, 10.0 - m_timer) * 0.25);
}
