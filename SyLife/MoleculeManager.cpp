#include "MoleculeManager.h"
#include "Molecule.h"
#include "FieldManager.h"

unique_ptr<MoleculeManager> g_moleculeManagerPtr;

int MoleculeManager::NumMolecule(const shared_ptr<MoleculeModel>& model)
{
	return static_cast<int>(count_if(GetMolecules().begin(), GetMolecules().end(), [&model](const auto& m) { return m->m_model == model; }));
}

const shared_ptr<Molecule>& MoleculeManager::AddMolecule(const shared_ptr<MoleculeModel>& model)
{
	const auto& m = GetMolecules().emplace_back(make_shared<Molecule>());

	m->m_model = model;
	m->m_radius = model->m_radius;
	m->m_mass = model->m_mass;

	return m;
}

const shared_ptr<Molecule>& MoleculeManager::AddMolecule(const shared_ptr<MoleculeModel>& model, const Vector2D & position)
{
	const auto& m = AddMolecule(model);

	m->m_position = position;

	return m;
}

void MoleculeManager::AddMoleculesRandom(const shared_ptr<MoleculeModel>& model, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		const auto& m = g_moleculeManagerPtr->AddMolecule(model);
		m->m_position.m_x = rand() % int(g_fieldManagerPtr->m_size.m_x);
		m->m_position.m_y = rand() % int(g_fieldManagerPtr->m_size.m_y);

		m->Init();
	}
}
