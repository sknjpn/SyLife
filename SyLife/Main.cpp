#include "FieldManager.h"
#include "CellManager.h"
#include "MoleculeManager.h"
#include "RigidbodySearcher.h"
#include "Cell.h"
#include "Molecule.h"

void Main()
{
	g_fieldManager = make_unique<FieldManager>();

	g_fieldManager->Init();

	// Moleculeの追加
	for (int i = 0; i <2000; i++)
	{
		const auto& m = g_moleculeManager->AddMolecule();

		m->m_radius = 4.0;
		m->m_mass = m->m_radius * m->m_radius * 1.0;
		m->m_position.m_x = s3d::Random(800);
		m->m_position.m_y = s3d::Random(600);

		m->Init();
	}

	// Cellの追加
	for (int i = 0; i < 50; i++)
	{
		const auto& c = g_cellManager->AddCell();

		c->m_radius = 32.0;
		c->m_mass = c->m_radius * c->m_radius * 1.0;
		c->m_position.m_x = s3d::Random(800);
		c->m_position.m_y = s3d::Random(600);

		c->Init();
	}

	g_rigidbodySearcher->m_index.buildIndex();

	while (s3d::System::Update())
	{
		g_fieldManager->Update();

		//if (s3d::KeyEnter.pressed())
		{
			g_cellManager->m_cells.front()->m_position.m_x = s3d::Cursor::PosF().x;
			g_cellManager->m_cells.front()->m_position.m_y = s3d::Cursor::PosF().y;
		}

		// Moleculeの描画
		for (const auto& m : g_moleculeManager->m_molecules)
		{
			s3d::Circle(m->m_position.m_x, m->m_position.m_y, m->m_radius).draw(s3d::Palette::Green).drawFrame(1.0, s3d::Palette::Black);
		}

		// Cellの描画
		for (const auto& c : g_cellManager->m_cells)
		{
			s3d::Circle(c->m_position.m_x, c->m_position.m_y, c->m_radius).draw(s3d::ColorF(s3d::Palette::Lightpink, 0.5)).drawFrame(1.0, s3d::Palette::Black);
			s3d::Circle(c->m_position.m_x, c->m_position.m_y, c->m_radius / 4.0).draw(s3d::Palette::Violet).drawFrame(1.0, s3d::Palette::Black);

			/*const auto& list = g_rigidbodySearcher->GetNearRigidbodies(c->m_position, c->m_radius * 2.0);

			for (auto l : list)
			{
				const auto& target = g_fieldManager->m_rigidbodies[l.first];
				s3d::Vec2 p1(c->m_position.m_x, c->m_position.m_y);
				s3d::Vec2 p2(target->m_position.m_x, target->m_position.m_y);

				s3d::Line(p1, p2).draw();
			}*/
		}
	}
}
