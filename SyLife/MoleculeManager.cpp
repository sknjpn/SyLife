#include "Molecule.h"

unique_ptr<MoleculeManager> g_moleculeManagerPtr;

const shared_ptr<MoleculeState>& MoleculeManager::AddMoleculeState(const shared_ptr<MoleculeModel>& model)
{
	const auto& m = GetMoleculeStates().emplace_back(make_shared<MoleculeState>());

	m->SetModel(model);

	return m;
}

const shared_ptr<MoleculeState>& MoleculeManager::AddMoleculeState(const shared_ptr<MoleculeModel>& model, const Vec2 & position)
{
	const auto& m = AddMoleculeState(model);
	m->SetPosition(position);
	m->SetVelocity(RandomVec2(Random(100.0)));

	return m;
}

void MoleculeManager::UpdateMoleculeStates()
{
	for (const auto& e : GetMoleculeStates())
	{
		if (!e->IsDestroyed())
		{
			e->UpdateParticle();
			e->UpdateMolecule();
		}
	}

	m_moleculeStates.remove_if([](const auto& m) { return m->IsDestroyed(); });
	m_moleculeStateKDTree.rebuildIndex();
}

void MoleculeManager::DrawMoleculeStates()
{
	for (const auto& e : GetMoleculeStates())
		if (!e->IsDestroyed()) e->Draw();
}

int MoleculeManager::NumMolecule(const shared_ptr<MoleculeModel>& model)
{
	return static_cast<int>(count_if(GetMoleculeStates().begin(), GetMoleculeStates().end(), [&model](const auto& m) { return m->GetModel() == model; }));
}

void MoleculeManager::AddMoleculesRandom(const shared_ptr<MoleculeModel>& model, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		auto p = RandomVec2(Random(1000));

		const auto& m = g_moleculeManagerPtr->AddMoleculeState(model);
		m->SetPosition(Vec2(p.x, p.y));

		m->Init();
	}
}

MoleculeStateAdapter::element_type MoleculeStateAdapter::GetElement(const dataset_type & dataset, size_t index, size_t dim) { return dataset[index]->GetPosition().elem(dim); }
MoleculeStateAdapter::element_type MoleculeStateAdapter::DistanceSq(const dataset_type & dataset, size_t index, const element_type * other) { return dataset[index]->GetPosition().distanceFromSq(Vec2(other[0], other[1])); }
