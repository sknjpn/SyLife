#include "CellManager.h"

void CellManager::Draw()
{
	static s3d::Font font(12);

	for (const auto& c : g_cellManagerPtr->GetCells())
	{
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(s3d::Vec2(c->m_position.m_x, c->m_position.m_y)));

			// parts
			for (const auto& p : c->m_model->m_parts)
			{
				auto tp = s3d::Transformer2D(s3d::Mat3x2::Rotate(p->m_rotation).translated(p->m_position.m_x, p->m_position.m_y));

				auto ar = p->m_model->m_approximateRect;
				s3d::RectF(ar.first.m_x, ar.first.m_y, ar.second.m_x - ar.first.m_x, ar.second.m_y - ar.first.m_y).draw(s3d::ColorF(s3d::Palette::Orange, 0.2)).drawFrame(1.0, s3d::Palette::Black);

				p->m_model->Draw();
			}

			// ×–E‰~
			//if (false)
			{
				double a = min(0.5, c->m_deathTimer * 0.25);

				s3d::Circle(c->m_radius).draw(s3d::ColorF(s3d::Palette::Lightpink, a)).drawFrame(1.0, s3d::Palette::Gray);
			}
		}

		// “à•”•ªŽq
		if (false)
		{
			string text;
			for (const auto& m : c->m_storage.m_molecules) text += m.first->m_name + to_string(m.second) + "\n";
			font(s3d::Unicode::Widen(text)).drawAt(c->m_position.m_x, c->m_position.m_y);
		}
	}
}