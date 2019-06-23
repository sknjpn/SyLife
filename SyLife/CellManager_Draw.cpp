#include "CellManager.h"

void CellManager::Draw()
{
	static s3d::Font font(12);

	for (const auto& c : g_cellManagerPtr->GetCells())
	{
		auto t1 = s3d::Transformer2D(s3d::Mat3x2::Rotate(c->m_rotation).translated(s3d::Vec2(c->m_position.m_x, c->m_position.m_y)));

		// parts
		for (const auto& p : c->m_parts)
		{
			auto t2 = s3d::Transformer2D(s3d::Mat3x2::Rotate(p->m_config->m_rotation).translated(p->m_config->m_position.m_x, p->m_config->m_position.m_y));

			// —Ìˆæ˜g
			if (false)
			{
				auto ar = p->m_config->m_model->m_approximateRect;
				s3d::RectF(ar.first.m_x, ar.first.m_y, ar.second.m_x - ar.first.m_x, ar.second.m_y - ar.first.m_y)
					.draw(s3d::ColorF(s3d::Palette::Orange, 0.2))
					.drawFrame(1.0, s3d::Palette::Black);
			}

			p->Draw(*c);
		}

		// ×–E‰~
		//if (false)
		{
			double a = min(0.5, c->m_deathTimer * 0.25);

			s3d::Circle(c->m_radius)
				.draw(s3d::ColorF(s3d::Palette::Lightpink, a))
				.drawFrame(1.0, s3d::Palette::Gray);
		}

		// “à•”•ªŽq
		if (false)
		{
			string text;

			for (const auto& m : c->m_storage.m_molecules) 
				text += m.first->m_name + to_string(m.second) + "\n";

			font(s3d::Unicode::Widen(text)).drawAt(s3d::Vec2::Zero());
		}
	}
}