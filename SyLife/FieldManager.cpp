#include "FieldManager.h"
#include "MoleculeManager.h"
#include "CellManager.h"
#include "RecipeManager.h"
#include "Rigidbody.h"

unique_ptr<FieldManager>	g_fieldManagerPtr;

FieldManager::FieldManager()
	: m_size(800, 600)
{

}

FieldManager::~FieldManager()
{

}

void FieldManager::Init()
{
	g_moleculeManagerPtr = make_unique<MoleculeManager>();
	g_cellManagerPtr = make_unique<CellManager>();
	g_recipeManagerPtr = make_unique<RecipeManager>();
}

void FieldManager::Update()
{
	for (const auto& r : m_indexer.GetParticles())
	{
		m_indexer.ForEachNearParticles(r->m_position, r->m_radius * 2.0, [r](const auto& p, double distance) {
			auto t = dynamic_pointer_cast<Rigidbody>(p);

			distance = sqrt(distance);
			if (t->m_position != r->m_position && distance - t->m_radius - r->m_radius < 0.0)
			{
				if (r->m_radius < t->m_radius) return;
				if (r->m_radius == t->m_radius && r < t) return;

				auto f = 64.0 * (distance - t->m_radius - r->m_radius) * (t->m_position - r->m_position) / distance;
				r->AddForceInWorld(f, r->m_position);
				t->AddForceInWorld(-f, t->m_position);
			}
		});

		// •Ài‰^“®
		r->m_position += r->m_velocity;

		// •Ç‚Å‚Ì”½ŽË
		if (r->m_position.m_x < 0 && r->m_velocity.m_x < 0) r->m_velocity.m_x = -r->m_velocity.m_x;
		if (r->m_position.m_y < 0 && r->m_velocity.m_y < 0) r->m_velocity.m_y = -r->m_velocity.m_y;
		if (r->m_position.m_x > m_size.m_x && r->m_velocity.m_x > 0) r->m_velocity.m_x = -r->m_velocity.m_x;
		if (r->m_position.m_y > m_size.m_y && r->m_velocity.m_y > 0) r->m_velocity.m_y = -r->m_velocity.m_y;

		// –€ŽC’ïR
		r->m_velocity /= (1.0 + r->m_radius * 0.001);
	}

	g_cellManagerPtr->Update();
	g_moleculeManagerPtr->Update();

	m_indexer.Update();
}