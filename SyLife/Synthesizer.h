#pragma once

#include "Module.h"
#include "Storage.h"
#include "Cell.h"

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
public:
	void	Draw(const CellState& cell) const { m_config->m_model->Draw(); }
	void	Update(CellState& cell) override
	{
		auto model = dynamic_pointer_cast<SynthesizerModel>(m_config->m_model);

		if (cell.m_storage >= model->GetImport() && cell.m_model->m_material.Num(model->GetExport()) > cell.m_storage.Num(model->GetExport()))
		{
			cell.m_storage -= model->GetImport();
			cell.m_storage.Add(model->GetExport());
		}
	}
};

inline shared_ptr<PartState>	SynthesizerModel::MakeState() { return make_shared<SynthesizerState>(); }

inline void SynthesizerModel::SetFromJSON(const ptree& pt)
{
	// import
	m_import.Load(pt.get_child("material"));

	// export
	m_export = g_assetManagerPtr->GetModel<MoleculeModel>(pt.get<string>("product"));

	ModuleModel::SetFromJSON(pt);
}
