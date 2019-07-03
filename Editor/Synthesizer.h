#pragma once

#include "Module.h"
#include "Storage.h"
#include "Cell.h"

class SynthesizerViewer;

class SynthesizerModel
	: public ModuleModel
{
	Storage	m_import;
	shared_ptr<MoleculeModel>	m_export;

public:
	const Storage&	GetImport() const { return m_import; }
	const shared_ptr<MoleculeModel>&	GetExport() const { return m_export; }

	void	MakeViewers() override;

	void	Load_this(const ptree& pt);
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const;
	void	Save(ptree& pt) const override { Save_this(pt); }
};

class SynthesizerViewer
	: public PartViewer
{
public:
	TextEditState		m_textEditState_name;
	TextEditState		m_textEditState_mass;

public:
	SynthesizerViewer(const shared_ptr<PartModel>& model)
		: PartViewer(model)
		, m_textEditState_name(Unicode::Widen(model->GetName()))
		, m_textEditState_mass(ToString(model->m_mass))
	{
		m_drawRect = RectF(0, 450, 600, 150);
	}
};

inline void SynthesizerModel::MakeViewers()
{
	g_viewerManagerPtr->MakeViewer<SynthesizerViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

inline void SynthesizerModel::Load_this(const ptree& pt)
{
	// import
	m_import.Load(pt.get_child("import"));

	// export
	m_export = g_assetManagerPtr->GetModel<MoleculeModel>(pt.get<string>("export"));

	ModuleModel::Load_this(pt);
}

inline void SynthesizerModel::Save_this(ptree& pt) const
{
	// import
	{
		ptree pt2;
		
		m_import.Save(pt2);

		pt.push_back(std::make_pair("import", pt2));
	}

	// export
	pt.put("export", m_export->GetName());

	ModuleModel::Save_this(pt);

	pt.put("type", "SynthesizerModel");
}