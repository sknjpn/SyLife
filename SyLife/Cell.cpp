#include "Cell.h"
#include "Part.h"
#include "Egg.h"
#include "EggManager.h"
#include "MoleculeManager.h"

#include <numeric>

void CellModel::Draw(double a)
{
	// parts
	for (const auto& pc : m_partConfigs)
	{
		auto t2 = Transformer2D(Mat3x2::Rotate(pc->GetRotation())
			.translated(pc->GetPosition().x, pc->GetPosition().y));

		pc->GetModel()->Draw(a);
	}
}

void CellModel::SetFromJSON(const ptree& pt)
{
	// parts
	for (auto part : pt.get_child("parts")) m_partConfigs.emplace_back(make_shared<PartConfig>())->Load(part.second);

	UpdateProperties();

	Model::SetFromJSON(pt);
}

void CellModel::AddToJSON(ptree& pt) const
{
	// parts
	{
		ptree parts;

		for (const auto& e : m_partConfigs)
		{
			ptree part; e->Save(part);
			parts.push_back(std::make_pair("", part));
		}

		pt.add_child("parts", parts);
	}

	Model::AddToJSON(pt);

	pt.put("type", "CellModel");
}

void CellModel::UpdateProperties()
{
	// mass
	m_mass = accumulate(m_partConfigs.begin(), m_partConfigs.end(), 0.0, [](double mass, const auto& p) { return mass + p->GetModel()->GetMass(); });

	// center
	Vec2 center = accumulate(m_partConfigs.begin(), m_partConfigs.end(), Vec2::Zero(), [](Vec2 acc, const auto& p) { return acc + p->GetModel()->GetMass() * (p->GetPosition() + p->GetModel()->GetCenter().rotated(p->GetRotation())); }) / m_mass;

	// center‚ğŒ´“_‚Éİ’è
	for (const auto& p : m_partConfigs) p->SetPosition(p->GetPosition() - center);

	// inertia
	m_inertia = accumulate(m_partConfigs.begin(), m_partConfigs.end(), 0.0, [](double acc, const auto& p) { return acc + p->GetInertia(); });

	// radius
	m_radius = sqrt(2 * m_inertia / m_mass);

	// material
	m_material = accumulate(m_partConfigs.begin(), m_partConfigs.end(), Storage(), [](Storage acc, const auto& p) { return acc += (p->GetModel()->GetMaterial()); });
}

CellState::CellState(const shared_ptr<CellModel>& model)
	: m_model(model)
	, m_startTimer(0.0)
	, m_deathTimer(25.0)
{
	SetMass(m_model->m_mass);
	SetRadius(m_model->m_radius);
	SetInertia(m_model->m_inertia);

	// parts
	for (const auto& pc : m_model->m_partConfigs)
	{
		const auto& ps = m_partStates.emplace_back(pc->GetModel()->MakeState());

		ps->SetPartConfig(pc);
	}
}

void CellState::UpdateCell()
{
	m_deathTimer -= g_fieldManagerPtr->GetDeltaTime();
	m_startTimer += g_fieldManagerPtr->GetDeltaTime();

	// parts
	for (const auto& p : m_partStates) p->Update(*this);

	// ÚG‚µ‚½MoleculeState‚Ìæ‚è‚İ
	for (const auto& m : g_moleculeManagerPtr->GetIndexer().GetNearParticles(GetPosition(), GetRadius() * 2.0))
		if (!m->IsDestroyed() && (m->GetPosition() - GetPosition()).length() - m->GetRadius() - GetRadius() < 0.0) TakeMolecule(m);

	// —]è‚ÌMoleculeState‚Ì“ŠŠü
	for (;;)
	{
		bool flag = true;

		for (const auto& m : m_storage.GetMolecules())
		{
			if (2 * m_model->m_material.Num(m.first) < m_storage.Num(m.first))
			{
				ExpireMolecule(m.first);

				flag = false;

				break;
			}
		}

		if (flag) break;
	}

	// •ª—ôˆ—
	if (m_storage >= m_model->m_material)
	{
		m_storage -= m_model->m_material;

		const auto& e = g_eggManagerPtr->AddEggState(m_model);
		e->SetPosition(GetPosition());
		e->SetRotation(Random(boost::math::constants::pi<double>() * 2.0));
		e->SetVelocity(Vec2(1.0, 0.0).rotated(rand() / 360.0));
	}

	// €–Sˆ—
	if (m_deathTimer <= 0.0)
	{
		// MoleculeState‚Ì“f‚«o‚µ
		auto s = m_storage + m_model->m_material;
		for (const auto& m : s.GetMolecules())
		{
			for (unsigned int i = 0; i < m.second; i++)
			{
				// “f‚«o‚·•ûŒü
				auto v = Vec2(1.0, 0.0).rotated(rand() / 3600.0);

				// “f‚«o‚³‚ê‚½MoleculeState
				const auto& ms = g_moleculeManagerPtr->AddMoleculeState(m.first);
				ms->SetPosition(GetPosition() + v * (GetRadius() + m.first->GetRadius()) * Random(1.0));
				ms->SetVelocity(v * 0.1);
			}
		}

		Destroy();
	}
}

void CellState::Draw()
{
	auto t1 = Transformer2D(Mat3x2::Rotate(GetRotation()).translated(Vec2(GetPosition().x, GetPosition().y)));
	auto t2 = Transformer2D(Mat3x2::Scale(min(1.0, m_startTimer + 0.5)));

	// parts
	for (const auto& p : m_partStates)
	{
		auto t3 = Transformer2D(Mat3x2::Rotate(p->GetPartConfig()->GetRotation())
			.translated(p->GetPartConfig()->GetPosition().x, p->GetPartConfig()->GetPosition().y));

		p->Draw(*this);
	}

	// ×–E‰~
	if (false)
	{
		double a = min(0.5, m_deathTimer * 0.25);

		Circle(GetRadius())
			.draw(ColorF(Palette::Lightpink, a))
			.drawFrame(1.0, Palette::Gray);
	}
}

void CellState::TakeMolecule(const shared_ptr<MoleculeState>& molecule)
{
	m_storage.Add(molecule->GetModel());

	molecule->Destroy();
}

void CellState::ExpireMolecule(const shared_ptr<MoleculeModel>& model, unsigned int size) 
{
	for (unsigned int i = 0; i < size; ++i) 
	{
		// “f‚«o‚·•ûŒü
		auto v = Vec2(1.0, 0.0).rotated(rand() / 3600.0);

		// “f‚«o‚³‚ê‚½MoleculeState
		const auto& t = g_moleculeManagerPtr->AddMoleculeState(model);
		t->SetPosition(GetPosition() + v * (GetRadius() + model->GetRadius()));
		t->SetVelocity(v * 0.5);

		// Storage‚©‚ço‚·
		m_storage.Pull(model);
	}
}