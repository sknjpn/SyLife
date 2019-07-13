#include "Cell.h"
#include "FieldManager.h"

unique_ptr<FieldManager>	g_fieldManagerPtr;

void FieldManager::Init()
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
	/*
	{
		for (auto cells : pt.get_child("cells"))
		{
			string name = cells.second.get<string>("name");
			int size = cells.second.get<int>("size");

			for (int i = 0; i < size; i++)
			{
				const auto& c = g_cellManagerPtr->AddCellState(g_assetManagerPtr->GetModel<CellModel>(name));
				c->SetPosition(Vec2(Random(m_size.x), Random(m_size.y)));
				c->SetVelocity(Vec2::Zero());

				c->Init();
			}
		}
	}
	*/
}

void FieldManager::Update()
{
	for (const auto& r : m_rigidbodies)
	{
		if (!r->IsDestroyed())
		{
			r->UpdateParticle();
			r->UpdateRigidbody();
		}
	}

	m_rigidbodies.remove_if([](const auto& r) { return r->IsDestroyed(); });
	m_rigidbodyKDTree.rebuildIndex();
}

RigidbodyAdapter::element_type RigidbodyAdapter::GetElement(const dataset_type & dataset, size_t index, size_t dim) { return dataset[index]->GetPosition().elem(dim); }
RigidbodyAdapter::element_type RigidbodyAdapter::DistanceSq(const dataset_type & dataset, size_t index, const element_type * other) { return dataset[index]->GetPosition().distanceFromSq(Vec2(other[0], other[1])); }
