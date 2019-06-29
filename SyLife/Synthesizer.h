#pragma once

#include "Module.h"
#include "Storage.h"
#include "Cell.h"

class SynthesizerModel
	: public ModuleModel
{
	Storage	m_material;
	Storage	m_product;

public:
	const Storage&	GetMaterial() const { return m_material; }
	const Storage&	GetProduct() const { return m_product; }

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
		
		if (cell.m_storage > model->GetMaterial())
		{
			cell.m_storage.PullMolecule(model->GetMaterial());
			cell.m_storage.AddStorage(model->GetProduct());
		}
	}
};

inline shared_ptr<PartState>	SynthesizerModel::MakeState() { return make_shared<SynthesizerState>(); }

inline void SynthesizerModel::SetFromJSON(const ptree& pt)
{
	ModuleModel::SetFromJSON(pt);
}
