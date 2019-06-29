#include "Cell.h"
#include "Part.h"
#include "MoleculeManager.h"
#include <numeric>

void CellModel::SetFromJSON(const ptree& pt)
{
	// parts
	for (auto part : pt.get_child("parts")) m_partConfigs.emplace_back(make_shared<PartConfig>())->Load(part.second);

	// mass
	m_mass = accumulate(m_partConfigs.begin(), m_partConfigs.end(), 0.0, [](double mass, const auto& p) { return mass + p->m_model->GetMass(); });

	// center
	Vector2D center = accumulate(m_partConfigs.begin(), m_partConfigs.end(), Vector2D::Zero(), [](Vector2D acc, const auto& p) { return acc + p->m_model->GetMass() * (p->m_position + p->m_model->GetCenter().rotated(p->m_rotation)); }) / m_mass;

	// centerを原点に設定
	for (const auto& p : m_partConfigs) p->m_position -= center;

	// inertia
	m_inertia = accumulate(m_partConfigs.begin(), m_partConfigs.end(), 0.0, [](double acc, const auto& p) { return acc + p->GetInertia(); });

	// radius
	m_radius = sqrt(2 * m_inertia / m_mass);

	// material
	m_material = accumulate(m_partConfigs.begin(), m_partConfigs.end(), Storage(), [](Storage acc, const auto& p) { acc.AddStorage(p->m_model->GetMaterial()); return acc; });

	Model::SetFromJSON(pt);
}

void CellState::Update()
{

}

void CellState::Draw()
{

}

void CellState::TakeMolecule(const shared_ptr<MoleculeState>& molecule)
{
	m_storage.Add(molecule->m_model);

	molecule->m_isDestroyed = true;
}

void CellState::ExpireMolecule(const shared_ptr<MoleculeModel>& model)
{
	// 吐き出す方向
	auto v = Vector2D(1.0, 0.0).rotated(rand() / 3600.0);

	// 吐き出されたMoleculeState
	const auto& t = g_moleculeManagerPtr->AddMoleculeState(model);
	t->m_position = m_position + v * (m_radius + model->GetRadius());
	t->m_velocity = v * 2.0;

	// Storageから出す
	m_storage.Pull(model);
}

void CellState::ExpireMolecule(const shared_ptr<MoleculeModel>& model, int size)
{
	for (int i = 0; i < size; ++i) ExpireMolecule(model);
}
