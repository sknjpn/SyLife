#include "MoleculeManager.h"
#include "Molecule.h"
#include "FieldManager.h"

unique_ptr<MoleculeManager> g_moleculeManagerPtr;

Indexer<MoleculeState>& MoleculeManager::GetIndexer()
{
	return m_indexer;
}

const Indexer<MoleculeState>& MoleculeManager::GetIndexer() const
{
	return m_indexer;
}

void MoleculeManager::Update()
{
	for (const auto& e : GetMoleculeStates())
	{
		if (!e->IsDestroyed())
		{
			e->UpdateParticle();
			e->UpdateMolecule();
		}
	}

	m_indexer.Update();
}

void MoleculeManager::Draw()
{
	for (const auto& e : GetMoleculeStates())
		if (!e->IsDestroyed()) e->Draw();
}

int MoleculeManager::NumMolecule(const shared_ptr<MoleculeModel>& model)
{
	return static_cast<int>(count_if(GetMoleculeStates().begin(), GetMoleculeStates().end(), [&model](const auto& m) { return m->GetModel() == model; }));
}

const shared_ptr<MoleculeState>& MoleculeManager::AddMoleculeState(const shared_ptr<MoleculeModel>& model)
{
	const auto& m = GetMoleculeStates().emplace_back(make_shared<MoleculeState>());

	m->SetModel(model);

	return m;
}

const shared_ptr<MoleculeState>& MoleculeManager::AddMoleculeState(const shared_ptr<MoleculeModel>& model, const s3d::Vec2 & position)
{
	const auto& m = AddMoleculeState(model);
	m->SetPosition(position);

	return m;
}

void MoleculeManager::AddMoleculesRandom(const shared_ptr<MoleculeModel>& model, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		auto p = s3d::RandomVec2(s3d::Random(1000));

		const auto& m = g_moleculeManagerPtr->AddMoleculeState(model);
		m->SetPosition(s3d::Vec2(p.x, p.y));

		m->Init();
	}
}
