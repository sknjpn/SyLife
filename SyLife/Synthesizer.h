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
	const Storage& GetImport() const { return m_import; }
	const shared_ptr<MoleculeModel>& GetExport() const { return m_export; }

	void	MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const
	{
		// import
		{
			ptree pt2;

			m_import.save(pt2);

			pt.push_back(std::make_pair("import", pt2));
		}

		// export
		pt.put("export", m_export->GetName());

		ModuleModel::save_this(pt);

		pt.put("type", "SynthesizerModel");
	}
	void	save(ptree& pt) const override { save_this(pt); }
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
		m_timer += g_systemManagerPtr->GetDeltaTime();

		auto model = dynamic_pointer_cast<SynthesizerModel>(GetPartConfig()->GetModel());
		if (m_timer > 2.0 && cell.m_storage >= model->GetImport() && cell.m_model->GetMaterial().numMolecule(model->GetExport()) > cell.m_storage.numMolecule(model->GetExport()))
		{
			m_timer = 0.0;

			cell.m_storage -= model->GetImport();
			cell.m_storage.addMolecule(model->GetExport(), 1);
		}
	}
};

class SynthesizerViewer
	: public ModuleViewer
{
public:
	// Reload
	void reloadProperties_this()
	{
		ModuleViewer::reloadProperties_this();
	}
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void Update_this()
	{
		ModuleViewer::Update_this();
	}
	void Update() override { Update_this(); }
};

inline void		SynthesizerModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<SynthesizerViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}

inline shared_ptr<PartState>	SynthesizerModel::MakeState() { return make_shared<SynthesizerState>(); }

inline void SynthesizerModel::load_this(const ptree& pt)
{
	// import
	m_import.load(pt.get_child("import"));

	// export
	m_export = g_assetManagerPtr->GetModel<MoleculeModel>(pt.get<string>("export"));

	ModuleModel::load_this(pt);
}
