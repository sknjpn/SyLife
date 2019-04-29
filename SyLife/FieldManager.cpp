#include "FieldManager.h"
#include "MoleculeManager.h"
#include "CellManager.h"
#include "Rigidbody.h"
#include "RigidbodySearcher.h"

unique_ptr<FieldManager>	g_fieldManager;

FieldManager::FieldManager()
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
		r->m_position += r->m_velocity;
	}

	g_rigidbodySearcher->m_index.buildIndex();
}