#include "Cell.h"
#include "Part.h"
#include "MoleculeManager.h"
#include <numeric>

void CellModel::SetFromJSON(const ptree & pt)
{
	// parts
	for (auto part : pt.get_child("parts")) AddPartConfig(part.second);

	CalculateDisk();

	Model::SetFromJSON(pt);
}


void CellModel::CalculateDisk()
{
	// mass
	{
		m_mass = 0.0;

		for (const auto& p : m_parts) m_mass += p->m_model->m_mass;
	}

	// center
	{
		// body
		Vector2D center(0.0, 0.0);

		for (const auto& p : m_parts) center += p->m_model->m_mass * (p->m_position + ((p->m_model->m_approximateRect.first + p->m_model->m_approximateRect.second) / 2.0).rotated(p->m_rotation));

		center /= m_mass;

		// 位置調整
		for (const auto& p : m_parts) p->m_position -= center;
	}

	// inertia
	{
		m_inertia = 0.0;

		for (const auto& p : m_parts) m_inertia += p->GetInertia();
	}

	// radius
	m_radius = sqrt(2 * m_inertia / m_mass);
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

void Cell::ExpireMolecule(const shared_ptr<MoleculeModel>& model)
{
	// 吐き出す方向
	auto v = Vector2D(1.0, 0.0).rotated(rand() / 3600.0);

	// 吐き出されたMolecule
	const auto& t = g_moleculeManagerPtr->AddMolecule(model);
	t->m_position = m_position + v * (m_radius + model->m_radius);
	t->m_velocity = v * 2.0;

	// Storageから出す
	m_storage.PullMolecule(model);

	// Moleculeのカウントを減らす
	m_molecules.PullMolecule(model);
}

void Cell::ExpireMolecule(const shared_ptr<MoleculeModel>& model, int size)
{
	for (int i = 0; i < size; ++i) ExpireMolecule(model);
}
