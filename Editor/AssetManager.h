#pragma once

#include "MoleculeModel.h"
#include "Part.h"
#include "CellModel.h"

class AssetManager
{
public:
	vector<shared_ptr<MoleculeModel>>	m_moleculeModels;
	vector<shared_ptr<PartModel>>		m_partModels;
	vector<shared_ptr<CellModel>>		m_cellModels;

public:
	AssetManager()
	{
		m_moleculeModels.reserve(1024);
		m_partModels.reserve(1024);
		m_cellModels.reserve(1024);
	}

	void	Init();

	const shared_ptr<MoleculeModel>&	GetMoleculeModel(const string& name) const
	{
		for (auto it = m_moleculeModels.begin(); it != m_moleculeModels.end(); ++it)
			if ((*it)->m_name == name) return *it;

		throw new exception;
	}
	const shared_ptr<PartModel>&	GetPartModel(const string& name) const
	{
		for (auto it = m_partModels.begin(); it != m_partModels.end(); ++it)
			if ((*it)->m_name == name) return *it;

		throw new exception;
	}
	const shared_ptr<CellModel>&	GetCellModel(const string& name) const
	{
		for (auto it = m_cellModels.begin(); it != m_cellModels.end(); ++it)
			if ((*it)->m_name == name) return *it;

		throw new exception;
	}
};

extern unique_ptr<AssetManager>	g_assetManagerPtr;