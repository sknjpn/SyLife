#pragma once

#include "Module.h"
#include "Storage.h"
#include "Cell.h"
#include "FieldManager.h"

class SynthesizerModel
	: public ModuleModel
{
	Storage	m_import;
	shared_ptr<MoleculeModel>	m_export;

public:
	const Storage&	GetImport() const { return m_import; }
	const shared_ptr<MoleculeModel>&	GetExport() const { return m_export; }

	shared_ptr<PartState>	MakeState() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class SynthesizerState
	: public ModuleState
{
	double	m_timer = 0.0;

public:
	void	Draw(const CellState& cell) const 
	{
		GetPartConfig()->GetModel()->Draw(min(m_timer / 2.0, 1.0) * 0.75 + 0.25);
	}

	void	Update(CellState& cell) override
	{
		m_timer += g_fieldManagerPtr->GetDeltaTime();

		auto model = dynamic_pointer_cast<SynthesizerModel>(GetPartConfig()->GetModel());
		if (m_timer > 2.0 && cell.m_storage >= model->GetImport() && cell.m_model->m_material.Num(model->GetExport()) > cell.m_storage.Num(model->GetExport()))
		{
			m_timer = 0.0;

			cell.m_storage -= model->GetImport();
			cell.m_storage.Add(model->GetExport());
		}
	}
};

inline shared_ptr<PartState>	SynthesizerModel::MakeState() { return make_shared<SynthesizerState>(); }

inline void SynthesizerModel::SetFromJSON(const ptree& pt)
{
	// import
	m_import.Load(pt.get_child("import"));

	// export
	m_export = g_assetManagerPtr->GetModel<MoleculeModel>(pt.get<string>("export"));

	ModuleModel::SetFromJSON(pt);
}
