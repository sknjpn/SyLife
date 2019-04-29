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
	for (int i = 0; i < 10000; i++)
	{
		const auto& m = g_moleculeManager->AddMolecule();

		m->m_radius = 4.0;
		m->m_mass = m->m_radius * m->m_radius * 2.0;
		m->m_position.m_x = s3d::Random(640);
		m->m_position.m_y = s3d::Random(480);

		{
			auto f = s3d::RandomVec2(1.0);

			m->AddForceInWorld(Vector2D(f.x, f.y)*m->m_mass * 2.0, m->m_position);
		}

		m->Init();
	}

	// Cellの追加
	for (int i = 0; i < 100; i++)
	{
		const auto& c = g_cellManager->AddCell();

		c->m_radius = 32.0;
		c->m_mass = c->m_radius * c->m_radius * 2.0;
		c->m_position.m_x = s3d::Random(640);
		c->m_position.m_y = s3d::Random(480);

		{
			auto f = s3d::RandomVec2(1.0);

			c->AddForceInWorld(Vector2D(f.x, f.y)*c->m_mass * 2.0, c->m_position);
		}

		c->Init();
	}

	while (s3d::System::Update())
	{
		g_fieldManager->Update();

		// Moleculeの描画
		for (const auto& m : g_moleculeManager->m_molecules)
		{
			s3d::Circle(m->m_position.m_x, m->m_position.m_y, m->m_radius).draw(s3d::Palette::Green).drawFrame(1.0, s3d::Palette::Black);
		}

		// Cellの描画
		for (const auto& c : g_cellManager->m_cells)
		{
			s3d::Circle(c->m_position.m_x, c->m_position.m_y, c->m_radius).draw(s3d::ColorF(s3d::Palette::Lightpink, 0.0)).drawFrame(1.0, s3d::Palette::Gray);
			s3d::Circle(c->m_position.m_x, c->m_position.m_y, c->m_radius / 4.0).draw(s3d::Palette::Violet).drawFrame(1.0, s3d::Palette::Black);

			const auto& list = g_rigidbodySearcher->GetNearRigidbodies(c->m_position, c->m_radius);

			for (auto l : list)
			{
				const auto& target = g_fieldManager->m_rigidbodies[l.first];
				s3d::Vec2 p1(c->m_position.m_x, c->m_position.m_y);
				s3d::Vec2 p2(target->m_position.m_x, target->m_position.m_y);

				s3d::Line(p1, p2).draw();
			}
		}
	}
}
