#include "Cell.h"
#include "MoleculeManager.h"


Cell::Cell()
{
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

void Cell::ExpireMolecule(const shared_ptr<Molecule::Model>& model) const
{
	auto v = Vector2D(1.0, 0.0).rotated(rand() / 360.0);
	const auto& t = g_moleculeManager->AddMolecule(model);
	t->m_position = m_position + v * (m_radius + model->m_radius);
	t->m_velocity = v * 1.0;
}
