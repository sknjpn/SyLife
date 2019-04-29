#include "FieldManager.h"
#include "MoleculeManager.h"
#include "CellManager.h"
#include "nanoflann.hpp"
#include "Rigidbody.h"

unique_ptr<FieldManager>	g_fieldManager;

FieldManager::FieldManager()
	: m_index(2, m_cloud, KDTreeSingleIndexAdaptorParams(20))
{
	g_moleculeManager = make_unique<MoleculeManager>();
	g_cellManager = make_unique<CellManager>();
}


FieldManager::~FieldManager()
{
}

void FieldManager::Init()
{
}

void FieldManager::Update()
{
	for (const auto& r : m_rigidbodies)
	{
		r->m_position += r->m_velocity;
	}

	m_index.buildIndex();
}

std::vector<std::pair<size_t, int>>  FieldManager::GetNearRigidbodies(Vector2D position, double radius) const
{
	const int query_pt[2] = { position.m_x, position.m_y };
	const int search_radius = radius * radius;
	std::vector<std::pair<size_t, int>>   ret_matches;
	nanoflann::SearchParams params;

	const size_t nMatches = m_index.radiusSearch(&query_pt[0], search_radius, ret_matches, params);

	return ret_matches;
}
