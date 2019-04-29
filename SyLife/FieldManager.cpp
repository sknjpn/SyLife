#include "FieldManager.h"
#include "MoleculeManager.h"
#include "CellManager.h"
#include "Rigidbody.h"
#include "RigidbodySearcher.h"

unique_ptr<FieldManager>	g_fieldManager;

FieldManager::FieldManager()
	: m_size(800, 600)
{

}

FieldManager::~FieldManager()
{

}

void FieldManager::Init()
{
	g_moleculeManager = make_unique<MoleculeManager>();
	g_cellManager = make_unique<CellManager>();
	g_rigidbodySearcher = make_unique<RigidbodySearcher>();
}

void FieldManager::Update()
{
	for (const auto& r : m_rigidbodies)
	{
		auto list = g_rigidbodySearcher->GetNearRigidbodies(r->m_position, r->m_radius * 2.0);

		for (const auto& l : list)
		{
			auto t = m_rigidbodies[l.first];
			auto length = (t->m_position - r->m_position).length();

			if (t != r && length - t->m_radius - r->m_radius < 0.0)
			{
				auto f = 64.0 * (length - t->m_radius - r->m_radius) * (t->m_position - r->m_position) / length;

				r->AddForceInWorld(f, r->m_position);
				t->AddForceInWorld(-f, t->m_position);
			}
		}

		if (r->m_position.m_x < 0 && r->m_velocity.m_x < 0) r->m_velocity.m_x = -r->m_velocity.m_x;
		if (r->m_position.m_y < 0 && r->m_velocity.m_y < 0) r->m_velocity.m_y = -r->m_velocity.m_y;
		if (r->m_position.m_x > m_size.m_x && r->m_velocity.m_x > 0) r->m_velocity.m_x = -r->m_velocity.m_x;
		if (r->m_position.m_y > m_size.m_y && r->m_velocity.m_y > 0) r->m_velocity.m_y = -r->m_velocity.m_y;

		r->m_velocity *= 0.99;
	}

	for (const auto& r : m_rigidbodies)
	{
		r->m_position += r->m_velocity;
	}

	g_rigidbodySearcher->m_index.buildIndex();
}