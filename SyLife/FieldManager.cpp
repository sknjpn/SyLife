#include "FieldManager.h"
#include "MoleculeManager.h"
#include "CellManager.h"

unique_ptr<FieldManager>	g_fieldManagerPtr;

void FieldManager::Init()
{
	LoadGenerationSetting();
}

int FieldManager::GetNumMoleculeAll() const
{
	int result = int(g_moleculeManagerPtr->GetMolecules().size());

	for (const auto& c : g_cellManagerPtr->GetCells())
	{
		for (const auto& m : c->m_molecules.m_molecules)
			result += m.second;
	}

	return result;
}
