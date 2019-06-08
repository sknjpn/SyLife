#include "FieldManager.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

#include "AssetManager.h"
#include "MoleculeManager.h"
#include "CellManager.h"

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

			g_moleculeManagerPtr->AddMoleculesRandom(g_assetManagerPtr->GetMoleculeModel(name), size);
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
				const auto& c = g_cellManagerPtr->AddCell();
				c->m_molecules.AddMolecule(g_assetManagerPtr->GetMoleculeModel("Amino Acid"), 5);
				c->m_molecules.AddMolecule(g_assetManagerPtr->GetMoleculeModel("Carbon"), 5);
				c->m_molecules.AddMolecule(g_assetManagerPtr->GetMoleculeModel("Oxygen"), 5);
				c->RecalculatePhysicalProperty();
				c->m_position.m_x = s3d::Random(800);
				c->m_position.m_y = s3d::Random(600);

				c->Init();
			}
		}
	}
}