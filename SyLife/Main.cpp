#include "FieldManager.h"
#include "CellManager.h"
#include "MoleculeManager.h"
#include "Searcher.h"
#include "Cell.h"
#include "Molecule.h"

void Main()
{
	g_fieldManagerPtr = make_unique<FieldManager>();

	g_fieldManagerPtr->Init();

	// MoleculeModelの追加

	const auto& carbon = g_moleculeManagerPtr->AddModel();
	carbon->m_name = "Carbon";
	carbon->m_mass = 12.0;
	carbon->m_radius = sqrt(carbon->m_mass);

	const auto& nitrogen = g_moleculeManagerPtr->AddModel();
	nitrogen->m_name = "Nitrogen";
	nitrogen->m_mass = 24.0;
	nitrogen->m_radius = sqrt(nitrogen->m_mass);

	const auto& oxygen = g_moleculeManagerPtr->AddModel();
	oxygen->m_name = "Oxygen";
	oxygen->m_mass = 36.0;
	oxygen->m_radius = sqrt(oxygen->m_mass);

	const auto& aminoAcid = g_moleculeManagerPtr->AddModel();
	aminoAcid->m_name = "Amino acid";
	aminoAcid->m_mass = oxygen->m_mass + carbon->m_mass + nitrogen->m_mass;
	aminoAcid->m_radius = sqrt(aminoAcid->m_mass);

	// Moleculeの追加
	g_moleculeManagerPtr->AddMoleculesRandom(oxygen, 1000);
	g_moleculeManagerPtr->AddMoleculesRandom(carbon, 1000);
	g_moleculeManagerPtr->AddMoleculesRandom(nitrogen, 500);
	g_moleculeManagerPtr->AddMoleculesRandom(aminoAcid, 0);

	// Cellの追加
	for (int i = 0; i < 10; i++)
	{
		const auto& c = g_cellManagerPtr->AddCell();
		c->m_molecules.AddMolecule(g_moleculeManagerPtr->GetModel("Amino acid"), 5);
		c->m_molecules.AddMolecule(g_moleculeManagerPtr->GetModel("Carbon"), 5);
		c->m_molecules.AddMolecule(g_moleculeManagerPtr->GetModel("Oxygen"), 5);
		c->RecalculatePhysicalProperty();
		c->m_position.m_x = s3d::Random(800);
		c->m_position.m_y = s3d::Random(600);

		c->Init();
	}

	g_particleSearcherPtr->m_index.buildIndex();
	s3d::Font font(12);
	s3d::Font printFont(16);

	shared_ptr<Rigidbody> selectedRigidbody = nullptr;

	while (s3d::System::Update())
	{
		/*
		s3d::ClearPrint();
		s3d::Print << g_fieldManagerPtr->m_rigidbodies.size();
		s3d::Print << g_cellManagerPtr->m_cells.size();
		s3d::Print << g_moleculeManagerPtr->m_molecules.size();
		*/

		g_fieldManagerPtr->Update();

		if (s3d::MouseR.pressed())
		{
			Vector2D cursorPos(s3d::Cursor::PosF().x, s3d::Cursor::PosF().y);

			for (auto l : g_particleSearcherPtr->GetNearParticles(cursorPos, 100))
			{
				const auto& target = g_fieldManagerPtr->m_rigidbodies[l.first];

				if (target->m_radius > (target->m_position - cursorPos).length() && dynamic_pointer_cast<Cell>(target) != nullptr)
				{
					const auto& c = dynamic_pointer_cast<Cell>(target);

					// Moleculeの吐き出し
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

			for (auto l : g_particleSearcherPtr->GetNearParticles(cursorPos, 100))
			{
				const auto& target = g_fieldManagerPtr->m_rigidbodies[l.first];

				if (target->m_radius > (target->m_position - cursorPos).length())
				{
					selectedRigidbody = target;
				}
			}
		}

		if (s3d::MouseL.pressed())
		{
			if (selectedRigidbody != nullptr) selectedRigidbody->m_position = Vector2D(s3d::Cursor::PosF().x, s3d::Cursor::PosF().y);
		}
		else selectedRigidbody = nullptr;

		// Moleculeの描画
		for (const auto& m : g_moleculeManagerPtr->m_molecules)
		{
			s3d::Color color = s3d::Palette::White;
			if (m->m_model == oxygen) color = s3d::Palette::Red;
			if (m->m_model == carbon) color = s3d::Palette::Skyblue;
			if (m->m_model == nitrogen) color = s3d::Palette::Green;
			if (m->m_model == aminoAcid) color = s3d::Palette::Yellow;

			s3d::Circle(m->m_position.m_x, m->m_position.m_y, m->m_radius).draw(color).drawFrame(1.0, s3d::Palette::Gray);
		}

		// Cellの描画
		for (const auto& c : g_cellManagerPtr->m_cells)
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

		s3d::Rect(192, 80).draw(s3d::ColorF(s3d::Palette::Gray, 0.75));
		for (int i = 0; i < g_moleculeManagerPtr->m_models.size(); ++i)
		{
			const auto& m = g_moleculeManagerPtr->m_models[i];
			s3d::Color color = s3d::Palette::White;
			if (m == oxygen) color = s3d::Palette::Red;
			if (m == carbon) color = s3d::Palette::Skyblue;
			if (m == nitrogen) color = s3d::Palette::Green;
			if (m == aminoAcid) color = s3d::Palette::Yellow;

			printFont(s3d::Unicode::Widen(m->m_name + " size=" + to_string(g_moleculeManagerPtr->NumMolecule(m)))).draw(0 + 1, i * 16 + 1, s3d::Palette::White);
			printFont(s3d::Unicode::Widen(m->m_name + " size=" + to_string(g_moleculeManagerPtr->NumMolecule(m)))).draw(0, i * 16, color);
		}
	}
}
