#include "FieldManager.h"
#include "SystemManager.h"
#include "CellManager.h"
#include "MoleculeManager.h"
#include "AssetManager.h"
#include "Cell.h"
#include "Molecule.h"

void Main()
{
	g_systemManagerPtr = make_unique<SystemManager>();

	// MoleculeModelの追加
	const auto& carbon = g_assetManagerPtr->GetMoleculeModel("Carbon");
	const auto& nitrogen = g_assetManagerPtr->GetMoleculeModel("Nitrogen");
	const auto& oxygen = g_assetManagerPtr->GetMoleculeModel("Oxygen");
	const auto& aminoAcid = g_assetManagerPtr->GetMoleculeModel("Amino Acid");

	// Moleculeの追加
	g_moleculeManagerPtr->AddMoleculesRandom(oxygen, 2000);
	g_moleculeManagerPtr->AddMoleculesRandom(carbon, 2000);
	g_moleculeManagerPtr->AddMoleculesRandom(nitrogen, 1000);
	g_moleculeManagerPtr->AddMoleculesRandom(aminoAcid, 0);

	// Cellの追加
	for (int i = 0; i < 10; i++)
	{
		const auto& c = g_cellManagerPtr->AddCell();
		c->m_molecules.AddMolecule(g_assetManagerPtr->GetMoleculeModel("Amino Acid"), 5);
		c->m_molecules.AddMolecule(g_assetManagerPtr->GetMoleculeModel("Carbon"), 5);
		c->m_molecules.AddMolecule(g_assetManagerPtr->GetMoleculeModel("Oxygen"), 5);
		c->RecalculatePhysicalProperty();
		c->m_position.m_x = s3d::Random(800);
		c->m_position.m_y = s3d::Random(600);

		c->Init();
	}

	s3d::Font font(12);
	s3d::Font printFont(16);

	shared_ptr<Rigidbody> selectedRigidbody = nullptr;

	while (s3d::System::Update())
	{
		g_systemManagerPtr->Update();
		g_systemManagerPtr->Draw();
	}
}
