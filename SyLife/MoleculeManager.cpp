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

int MoleculeManager::NumMolecule(const shared_ptr<MoleculeModel>& model)
{
	return static_cast<int>(count_if(GetMoleculeStates().begin(), GetMoleculeStates().end(), [&model](const auto& m) { return m->m_model == model; }));
}

const shared_ptr<MoleculeState>& MoleculeManager::AddMoleculeState(const shared_ptr<MoleculeModel>& model)
{
	const auto& m = GetMoleculeStates().emplace_back(make_shared<MoleculeState>());

	m->m_model = model;
	m->m_radius = model->GetRadius();
	m->m_mass = model->GetMass();

	return m;
}

const shared_ptr<MoleculeState>& MoleculeManager::AddMoleculeState(const shared_ptr<MoleculeModel>& model, const Vector2D & position)
{
	const auto& m = AddMoleculeState(model);

	m->m_position = position;

	return m;
}

void MoleculeManager::AddMoleculesRandom(const shared_ptr<MoleculeModel>& model, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		const auto& m = g_moleculeManagerPtr->AddMoleculeState(model);
		m->m_position.m_x = rand() % int(g_fieldManagerPtr->GetSize().m_x);
		m->m_position.m_y = rand() % int(g_fieldManagerPtr->GetSize().m_y);

		m->Init();
	}
}
