#pragma once

#include "Molecule.h"
#include "Part.h"
#include "Cell.h"

class AssetManager
{
public:
	vector<shared_ptr<MoleculeModel>>	m_moleculeModels;
	vector<shared_ptr<PartModel>>	m_partModels;
	vector<shared_ptr<CellModel>>	m_cellModels;

public:
};

extern unique_ptr<AssetManager>	g_assetManagerPtr;