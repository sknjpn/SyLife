#include "CellManager.h"

void CellManager::Draw()
{
	static s3d::Font font(12);

	for (const auto& c : g_cellManagerPtr->GetCells())
	{
		double a = min(0.5, c->m_deathTimer * 0.25);
		s3d::Circle(c->m_position.m_x, c->m_position.m_y, c->m_radius).draw(s3d::ColorF(s3d::Palette::Lightpink, a)).drawFrame(1.0, s3d::Palette::Gray);
		s3d::Circle(c->m_position.m_x, c->m_position.m_y, c->m_radius / 4.0).draw(s3d::Palette::Violet).drawFrame(1.0, s3d::Palette::Gray);

		string text;
		for (const auto& m : c->m_storage.m_molecules) text += m.first->m_name + to_string(m.second) + "\n";
		font(s3d::Unicode::Widen(text)).drawAt(c->m_position.m_x, c->m_position.m_y);
		/*const auto& list = g_particleSearcherPtr->GetNearParticles(c->m_position, c->m_radius * 2.0);

		for (auto l : list)
		{
			const auto& target = g_fieldManagerPtr->m_rigidbodies[l.first];
			s3d::Vec2 p1(c->m_position.m_x, c->m_position.m_y);
			s3d::Vec2 p2(target->m_position.m_x, target->m_position.m_y);

			s3d::Line(p1, p2).draw();
		}*/
	}
}