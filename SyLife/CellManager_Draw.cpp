#include "CellManager.h"
#include "Cell.h"

void CellManager::Draw()
{
	static s3d::Font font(12);

	for (const auto& c : g_cellManagerPtr->GetCellStates())
	{
		auto t1 = s3d::Transformer2D(s3d::Mat3x2::Rotate(c->m_rotation).translated(s3d::Vec2(c->m_position.m_x, c->m_position.m_y)));
		auto t2 = s3d::Transformer2D(s3d::Mat3x2::Scale(min(1.0, c->m_startTimer + 0.5)));

		// parts
		for (const auto& p : c->m_partStates)
		{
			auto t2 = s3d::Transformer2D(s3d::Mat3x2::Rotate(p->m_config->m_rotation).translated(p->m_config->m_position.m_x, p->m_config->m_position.m_y));

			p->Draw(*c);
		}

		// ×–E‰~
		if (false)
		{
			double a = min(0.5, c->m_deathTimer * 0.25);

			s3d::Circle(c->m_radius)
				.draw(s3d::ColorF(s3d::Palette::Lightpink, a))
				.drawFrame(1.0, s3d::Palette::Gray);
		}
	}
}