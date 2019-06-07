#include "Cell.h"
#include "Part.h"
#include "MoleculeManager.h"
#include <numeric>

Cell::Cell()
{
	m_deathTimer = 10.0;
}

Cell::~Cell()
{

}

void Cell::RecalculatePhysicalProperty()
{
	m_mass = accumulate(m_molecules.m_molecules.begin(), m_molecules.m_molecules.end(), 0.0, [](double s, const auto& m) { return s + m.first->m_mass * m.second; });
	m_radius = sqrt(m_mass);
}

void Cell::Update()
{
	m_body->Update();

	for (const auto& m : m_modules) m->Update();
	for (const auto& e : m_equipments) e->Update();
}

void Cell::Draw()
{
	m_body->Draw();

	for (const auto& m : m_modules) m->Draw();
	for (const auto& e : m_equipments) e->Draw();
}

void Cell::TakeMolecule(const shared_ptr<Molecule>& molecule)
{
	m_mass += molecule->m_mass;
	m_radius = sqrt(m_mass);

	m_storage.AddMolecule(molecule->m_model);
	m_molecules.AddMolecule(molecule->m_model);

	molecule->m_destroyFlag = true;
}

void Cell::ExpireMolecule(const shared_ptr<MoleculeModel>& model)
{
	m_mass -= model->m_mass;
	m_radius = sqrt(m_mass);

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
