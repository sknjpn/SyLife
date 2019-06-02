#include "MoleculeManager.h"
#include "Molecule.h"
#include "FieldManager.h"

unique_ptr<MoleculeManager> g_moleculeManagerPtr;

MoleculeManager::MoleculeManager()
{
	m_models.reserve(1000);
}


MoleculeManager::~MoleculeManager()
{
}

int MoleculeManager::NumMolecule(const shared_ptr<Molecule::Model>& model)
{
	return static_cast<int>(count_if(GetMolecules().begin(), GetMolecules().end(), [&model](const auto& m) { return m->m_model == model; }));
}

const shared_ptr<Molecule>& MoleculeManager::AddMolecule(const shared_ptr<Molecule::Model>& model)
{
	const auto& m = GetMolecules().emplace_back(make_shared<Molecule>());

	m->m_model = model;
	m->m_radius = model->m_radius;
	m->m_mass = model->m_mass;

	m_indexer.AddParticle(m);
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
	for (const auto& m : GetMolecules())
	{
		if (m->m_model == GetModel("Amino Acid") && rand() % 100 == 0)
		{
			AddMolecule(GetModel("Nitrogen"), m->m_position + Vector2D(1.0, 0.0).rotated(rand() / 360.0));
			AddMolecule(GetModel("Carbon"), m->m_position + Vector2D(1.0, 0.0).rotated(rand() / 360.0));
			AddMolecule(GetModel("Oxygen"), m->m_position + Vector2D(1.0, 0.0).rotated(rand() / 360.0));

			m->m_destroyFlag = true;
		}

		// •Ài‰^“®
		m->m_position += m->m_velocity;

		// •Ç‚Å‚Ì”½ŽË
		if (m->m_position.m_x < 0 && m->m_velocity.m_x < 0) m->m_velocity.m_x = -m->m_velocity.m_x;
		if (m->m_position.m_y < 0 && m->m_velocity.m_y < 0) m->m_velocity.m_y = -m->m_velocity.m_y;
		if (m->m_position.m_x > g_fieldManagerPtr->m_size.m_x && m->m_velocity.m_x > 0) m->m_velocity.m_x = -m->m_velocity.m_x;
		if (m->m_position.m_y > g_fieldManagerPtr->m_size.m_y && m->m_velocity.m_y > 0) m->m_velocity.m_y = -m->m_velocity.m_y;

		// –€ŽC’ïR
		m->m_velocity /= (1.0 + m->m_radius * 0.001);
	}

	GetMolecules().erase(remove_if(GetMolecules().begin(), GetMolecules().end(), [](const auto& m) { return m->m_destroyFlag; }), GetMolecules().end());
	m_indexer.Update();
}
