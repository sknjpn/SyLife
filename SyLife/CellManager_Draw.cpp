#include "CellManager.h"

void CellManager::Draw()
{
	static s3d::Font font(12);

	for (const auto& c : g_cellManagerPtr->GetCells())
	{
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(s3d::Vec2(c->m_position.m_x, c->m_position.m_y)));
			auto ts = s3d::Transformer2D(s3d::Mat3x2::Scale(0.4));

			// body
			{
				c->m_model->m_body->m_model->Draw();
			}

			// equipments
			for (const auto& e : c->m_model->m_equipments)
			{
				auto tp = s3d::Transformer2D(s3d::Mat3x2::Translate(e->m_position));

				e->m_model->Draw();
			}

			// modules
			for (const auto& m : c->m_model->m_modules)
			{
				auto tp = s3d::Transformer2D(s3d::Mat3x2::Translate(m->m_position));

				m->m_model->Draw();
			}
		}

		// ×–E‰~
		if (false)
		{
			double a = min(0.5, c->m_deathTimer * 0.25);

			s3d::Circle(c->m_position.m_x, c->m_position.m_y, c->m_radius).draw(s3d::ColorF(s3d::Palette::Lightpink, a)).drawFrame(1.0, s3d::Palette::Gray);
			s3d::Circle(c->m_position.m_x, c->m_position.m_y, c->m_radius / 4.0).draw(s3d::Palette::Violet).drawFrame(1.0, s3d::Palette::Gray);
		}

		// “à•”•ªŽq
		{
			string text;
			for (const auto& m : c->m_storage.m_molecules) text += m.first->m_name + to_string(m.second) + "\n";
			font(s3d::Unicode::Widen(text)).drawAt(c->m_position.m_x, c->m_position.m_y);
		}
	}
}