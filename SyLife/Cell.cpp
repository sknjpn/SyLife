#include "Cell.h"
#include "MoleculeManager.h"

Cell::Cell()
{
	m_deathTimer = 10.0;
}

Cell::~Cell()
{

}

void Cell::Update()
{
}

void Cell::Draw()
{
}

void Cell::TakeMolecule(const shared_ptr<Molecule>& molecule)
{
	m_storage.AddMolecule(molecule->m_model);
	m_molecules.AddMolecule(molecule->m_model);

	molecule->m_destroyFlag = true;
}

void Cell::ExpireMolecule(const shared_ptr<Molecule::Model>& model) const
{
	auto v = Vector2D(1.0, 0.0).rotated(rand() / 3600.0);
	const auto& t = g_moleculeManagerPtr->AddMolecule(model);

	t->m_position = m_position + v * (m_radius + model->m_radius);
	t->m_velocity = v * 1.0;
}

void Cell::ExpireMolecule(const shared_ptr<Molecule::Model>& model, int size) const
{
	for (int i = 0; i < size; ++i) ExpireMolecule(model);
}
