#pragma once

#include "CellModel.h"
#include "MoleculeModel.h"
#include "PartModel.h"

#include "BodyModel.h"
#include "EquipmentModel.h"
#include "ModuleModel.h"

class AssetManager
{
public:
	vector<shared_ptr<MoleculeModel>>	m_moleculeModels;
	vector<shared_ptr<PartModel>>		m_partModels;
	vector<shared_ptr<CellModel>>		m_cellModels;

	vector<shared_ptr<BodyModel>>		m_bodyModels;
	vector<shared_ptr<EquipmentModel>>	m_equipmentModels;
	vector<shared_ptr<ModuleModel>>		m_moduleModels;

public:
	AssetManager()
	{
		m_moleculeModels.reserve(1024);
		m_partModels.reserve(1024);
		m_cellModels.reserve(1024);

		m_bodyModels.reserve(1024);
		m_equipmentModels.reserve(1024);
		m_moduleModels.reserve(1024);
	}

	void	Init();

	const shared_ptr<MoleculeModel>&	GetMoleculeModel(const string& name) const;
	const shared_ptr<PartModel>&		GetPartModel(const string& name) const;
	const shared_ptr<BodyModel>&		GetBodyModel(const string& name) const;
	const shared_ptr<EquipmentModel>&	GetEquipmentModel(const string& name) const;
	const shared_ptr<ModuleModel>&		GetModuleModel(const string& name) const;
	const shared_ptr<CellModel>&		GetCellModel(const string& name) const;
};

extern unique_ptr<AssetManager>	g_assetManagerPtr;