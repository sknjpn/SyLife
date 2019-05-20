#include "MoleculeManager.h"
#include "Molecule.h"
#include "FieldManager.h"
#include "ParticleSearcher.h"

unique_ptr<MoleculeManager> g_moleculeManagerPtr;

MoleculeManager::MoleculeManager()
{
	m_molecules.reserve(10000);
	m_models.reserve(1000);
}


MoleculeManager::~MoleculeManager()
{
}

int MoleculeManager::NumMolecule(const shared_ptr<Molecule::Model>& model)
{
	return static_cast<int>(count_if(m_molecules.begin(), m_molecules.end(), [&model](const auto& m) { return m->m_model == model; }));
}

const shared_ptr<Molecule>& MoleculeManager::AddMolecule(const shared_ptr<Molecule::Model>& model)
{
	const auto& m = m_molecules.emplace_back(make_shared<Molecule>());

	m->m_model = model;
	m->m_radius = model->m_radius;
	m->m_mass = model->m_mass;

	g_fieldManagerPtr->m_rigidbodies.emplace_back(m);
	g_particleSearcherPtr->m_cloud.m_particles.emplace_back(m);
	return m;
}

const shared_ptr<Molecule>& MoleculeManager::AddMolecule(const shared_ptr<Molecule::Model>& model, const Vector2D & position)
{
	const auto& m = AddMolecule(model);

	m->m_position = position;

	return m;
}

void MoleculeManager::AddMoleculesRandom(const shared_ptr<Molecule::Model>& model, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		const auto& m = g_moleculeManagerPtr->AddMolecule(model);
		m->m_position.m_x = rand() % int(g_fieldManagerPtr->m_size.m_x);
		m->m_position.m_y = rand() % int(g_fieldManagerPtr->m_size.m_y);

		m->Init();
	}
}

const shared_ptr<Molecule::Model>& MoleculeManager::AddModel()
{
	const auto& m = m_models.emplace_back(make_shared<Molecule::Model>());

	return m;
}

const shared_ptr<Molecule::Model>& MoleculeManager::GetModel(const string& name) const
{
	for (auto it = m_models.begin(); it != m_models.end(); ++it)
	{
		if ((*it)->m_name == name) return *it;
	}

	throw new exception;
}

void MoleculeManager::Update()
{
	for (const auto& m : m_molecules)
	{
		if (m->m_model == GetModel("Amino acid") && rand() % 100 == 0)
		{
			AddMolecule(GetModel("Nitrogen"), m->m_position + Vector2D(1.0, 0.0).rotated(rand() / 360.0));
			AddMolecule(GetModel("Carbon"), m->m_position + Vector2D(1.0, 0.0).rotated(rand() / 360.0));
			AddMolecule(GetModel("Oxygen"), m->m_position + Vector2D(1.0, 0.0).rotated(rand() / 360.0));

			m->m_destroyFlag = true;
		}
	}

	m_molecules.erase(remove_if(m_molecules.begin(), m_molecules.end(), [](const auto& m) { return m->m_destroyFlag; }), m_molecules.end());
}
