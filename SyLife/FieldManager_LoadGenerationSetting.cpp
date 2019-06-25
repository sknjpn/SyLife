#include "FieldManager.h"
#include <boost/property_tree/json_parser.hpp>

#include "AssetManager.h"
#include "MoleculeManager.h"
#include "CellManager.h"

#include "Cell.h"

using namespace boost::property_tree;

void FieldManager::LoadGenerationSetting()
{
	ptree pt;
	read_json("assets/generation.json", pt);

	// molecules
	{
		for (auto molecules : pt.get_child("molecules"))
		{
			string name = molecules.second.get<string>("name");
			int size = molecules.second.get<int>("size");

			g_moleculeManagerPtr->AddMoleculesRandom(g_assetManagerPtr->GetModel<MoleculeModel>(name), size);
		}
	}

	//cells
	{
		for (auto cells : pt.get_child("cells"))
		{
			string name = cells.second.get<string>("name");
			int size = cells.second.get<int>("size");

			for (int i = 0; i < size; i++)
			{
				const auto& c = g_cellManagerPtr->AddCell(g_assetManagerPtr->GetModel<CellModel>(name));
				c->m_molecules.AddMolecule(g_assetManagerPtr->GetModel<MoleculeModel>("Amino Acid"), 5);
				c->m_molecules.AddMolecule(g_assetManagerPtr->GetModel<MoleculeModel>("Carbon"), 5);
				c->m_molecules.AddMolecule(g_assetManagerPtr->GetModel<MoleculeModel>("Oxygen"), 5);
				c->m_position.m_x = s3d::Random(m_size.m_x);
				c->m_position.m_y = s3d::Random(m_size.m_y);
				c->m_velocity = Vector2D::Zero();

				c->Init();
			}
		}
	}
}