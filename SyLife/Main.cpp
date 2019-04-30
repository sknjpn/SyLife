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

	// MoleculeModelの追加
	const auto& oxygen = g_moleculeManager->AddModel();
	oxygen->m_name = "Oxygen";
	oxygen->m_mass = 12.0;
	oxygen->m_radius = sqrt(oxygen->m_mass);

	const auto& carbon = g_moleculeManager->AddModel();
	carbon->m_name = "Carbon";
	carbon->m_mass = 9.0;
	carbon->m_radius = sqrt(carbon->m_mass);

	const auto& nitrogen = g_moleculeManager->AddModel();
	nitrogen->m_name = "Nitrogen";
	nitrogen->m_mass = 14.0;
	nitrogen->m_radius = sqrt(nitrogen->m_mass);

	const auto& aminoAcid = g_moleculeManager->AddModel();
	aminoAcid->m_name = "Amino acid";
	aminoAcid->m_mass = oxygen->m_mass + carbon->m_mass + nitrogen->m_mass;
	aminoAcid->m_radius = sqrt(aminoAcid->m_mass);

	// Moleculeの追加
	g_moleculeManager->AddMoleculesRandom(oxygen, 500);
	g_moleculeManager->AddMoleculesRandom(carbon, 1000);
	g_moleculeManager->AddMoleculesRandom(nitrogen, 500);
	g_moleculeManager->AddMoleculesRandom(aminoAcid, 100);

	// Cellの追加
	for (int i = 0; i < 10; i++)
	{
		const auto& c = g_cellManager->AddCell();

		c->m_radius = 32.0;
		c->m_mass = c->m_radius * c->m_radius * 1.0;
		c->m_position.m_x = s3d::Random(800);
		c->m_position.m_y = s3d::Random(600);

		c->Init();
	}

	g_rigidbodySearcher->m_index.buildIndex();
	s3d::Font font(12);

	while (s3d::System::Update())
	{
		g_fieldManager->Update();

		if (s3d::MouseL.pressed())
		{
			g_cellManager->m_cells.front()->m_position.m_x = s3d::Cursor::PosF().x;
			g_cellManager->m_cells.front()->m_position.m_y = s3d::Cursor::PosF().y;
		}

		// Moleculeの描画
		for (const auto& m : g_moleculeManager->m_molecules)
		{
			s3d::Color color = s3d::Palette::White;
			if (m->m_model == oxygen) color = s3d::Palette::Red;
			if (m->m_model == carbon) color = s3d::Palette::Black;
			if (m->m_model == nitrogen) color = s3d::Palette::Green;
			if (m->m_model == aminoAcid) color = s3d::Palette::Yellow;

			s3d::Circle(m->m_position.m_x, m->m_position.m_y, m->m_radius).draw(color).drawFrame(1.0, s3d::Palette::Gray);
		}

		// Cellの描画
		for (const auto& c : g_cellManager->m_cells)
		{
			s3d::Circle(c->m_position.m_x, c->m_position.m_y, c->m_radius).draw(s3d::ColorF(s3d::Palette::Lightpink, 0.5)).drawFrame(1.0, s3d::Palette::Gray);
			s3d::Circle(c->m_position.m_x, c->m_position.m_y, c->m_radius / 4.0).draw(s3d::Palette::Violet).drawFrame(1.0, s3d::Palette::Gray);

			string text;
			for (const auto& m : c->m_storage.m_molecules) text += m.first->m_name + to_string(m.second) + "\n";
			font(s3d::Unicode::Widen(text)).drawAt(c->m_position.m_x, c->m_position.m_y);
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
