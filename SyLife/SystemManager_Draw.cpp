#include "SystemManager.h"
#include "FieldManager.h"
#include "CellManager.h"
#include "MoleculeManager.h"
#include "AssetManager.h"
#include "Cell.h"
#include "Molecule.h"

void SystemManager::Draw()
{
	static s3d::Font font(12);
	static s3d::Font printFont(16);

	static shared_ptr<Rigidbody> selectedRigidbody = nullptr;

	if (s3d::MouseR.pressed())
	{
		Vector2D cursorPos(s3d::Cursor::PosF().x, s3d::Cursor::PosF().y);

		for (const auto& c : g_cellManagerPtr->m_indexer.GetNearParticles(cursorPos, 100))
		{
			if (c->m_radius > (c->m_position - cursorPos).length())
			{
				// Molecule‚Ì“f‚«o‚µ
				for (const auto& m : c->m_molecules.m_molecules)
					for (int i = 0; i < m.second; i++)
						g_moleculeManagerPtr->AddMolecule(m.first)->m_position = c->m_position + Vector2D((rand() % 100) / 100.0 * c->m_radius, 0.0).rotated(rand() / 3600.0);

				c->m_destroyFlag = true;
			}
		}
	}

	if (s3d::MouseL.down())
	{
		Vector2D cursorPos(s3d::Cursor::PosF().x, s3d::Cursor::PosF().y);

		for (auto target : g_fieldManagerPtr->m_indexer.GetNearParticles(cursorPos, 100))
		{
			if (target->m_radius > (target->m_position - cursorPos).length()) selectedRigidbody = target;
		}
	}

	if (s3d::MouseL.pressed())
	{
		if (selectedRigidbody != nullptr) selectedRigidbody->m_position = Vector2D(s3d::Cursor::PosF().x, s3d::Cursor::PosF().y);
	}
	else selectedRigidbody = nullptr;

	// Molecule‚Ì•`‰æ
	for (const auto& m : g_moleculeManagerPtr->GetMolecules())
	{
		s3d::Color color(m->m_model->m_color[0], m->m_model->m_color[1], m->m_model->m_color[2]);
		s3d::Circle(m->m_position.m_x, m->m_position.m_y, m->m_radius).draw(color).drawFrame(1.0, s3d::Palette::Gray);
	}

	// Cell‚Ì•`‰æ
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

	// UI‚Ì•`‰æ
	s3d::Rect(192, 80).draw(s3d::ColorF(s3d::Palette::Gray, 0.75));
	for (int i = 0; i < g_assetManagerPtr->m_moleculeModels.size(); ++i)
	{
		const auto& m = g_assetManagerPtr->m_moleculeModels[i];
		s3d::Color color(m->m_color[0], m->m_color[1], m->m_color[2]);

		printFont(s3d::Unicode::Widen(m->m_name + " size=" + to_string(g_moleculeManagerPtr->NumMolecule(m)))).draw(0 + 1, i * 16 + 1, s3d::Palette::White);
		printFont(s3d::Unicode::Widen(m->m_name + " size=" + to_string(g_moleculeManagerPtr->NumMolecule(m)))).draw(0, i * 16, color);
	}
}