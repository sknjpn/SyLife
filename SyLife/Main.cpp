#include "FieldManager.h"
#include "CellManager.h"
#include "MoleculeManager.h"
#include "Cell.h"
#include "Molecule.h"

void Main()
{
	g_fieldManager = make_unique<FieldManager>();

	g_fieldManager->Init();

	// Moleculeの追加
	for (int i = 0; i < 1000; i++)
	{
		const auto& m = g_moleculeManager->AddMolecule();

		m->m_mass = 1.0;
		m->m_radius = 2.0;
		m->m_position.m_x = s3d::Random(640);
		m->m_position.m_y = s3d::Random(480);
		
		{
			auto f = s3d::RandomVec2(1.0);

			m->AddForceInWorld(Vector2D(f.x, f.y)*5.0, m->m_position);
		}
	}

	// Cellの追加
	for (int i = 0; i < 100; i++)
	{
		const auto& c = g_cellManager->AddCell();

		c->m_position.m_x = s3d::Random(640);
		c->m_position.m_y = s3d::Random(480);
	}

	while (s3d::System::Update())
	{
		g_fieldManager->Update();

		// Moleculeの描画
		for (const auto& m : g_moleculeManager->m_molecules)
		{
			s3d::Circle(m->m_position.m_x, m->m_position.m_y, 2).draw(s3d::Palette::Green).drawFrame(1.0, s3d::Palette::Black);
		}

		// Cellの描画
		for (const auto& c : g_cellManager->m_cells)
		{
			s3d::Circle(c->m_position.m_x, c->m_position.m_y, 16).draw(s3d::Palette::Lightpink).drawFrame(1.0, s3d::Palette::Black);
			s3d::Circle(c->m_position.m_x, c->m_position.m_y, 4).draw(s3d::Palette::Violet).drawFrame(1.0, s3d::Palette::Black);
		}
	}
}
