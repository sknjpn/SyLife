#include "FieldManager.h"
#include "CellManager.h"
#include "MoleculeManager.h"
#include "Cell.h"
#include "Molecule.h"

void Main()
{
	g_fieldManager = make_unique<FieldManager>();

	g_fieldManager->Init();

	while (s3d::System::Update())
	{
		g_fieldManager->Update();

		// Moleculeの描画
		for (const auto& m : g_moleculeManager->m_molecules)
		{
			s3d::Circle(m->m_position.m_x, m->m_position.m_y, 8).draw(s3d::Palette::Green);
		}

		// Cellの描画
		for (const auto& c : g_cellManager->m_cells)
		{
			s3d::Circle(c->m_position.m_x, c->m_position.m_y, 16).draw(s3d::Palette::Pink);
		}
	}
}
