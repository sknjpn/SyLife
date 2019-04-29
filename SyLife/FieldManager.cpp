#include "FieldManager.h"
#include "MoleculeManager.h"
#include "CellManager.h"
#include "nanoflann.hpp"
#include "Rigidbody.h"

unique_ptr<FieldManager>	g_fieldManager;

FieldManager::FieldManager()
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
}
