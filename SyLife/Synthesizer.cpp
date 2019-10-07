#include "Synthesizer.h"

#include "Molecule.h"
#include "ViewerManager.h"
#include "AssetManager.h"
#include "SystemManager.h"

void SynthesizerModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<SynthesizerViewer>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> SynthesizerModel::MakeState()
{
	return make_shared<SynthesizerState>();
}

void SynthesizerModel::load_this(const ptree& pt)
{
	// import
	m_import.load(pt.get_child("import"));

	// export
	m_export = g_assetManagerPtr->getModel<MoleculeModel>(pt.get<string>("export"));

	ModuleModel::load_this(pt);
}

void SynthesizerModel::save_this(ptree& pt) const
{
	// import
	{
		ptree pt2;

		m_import.save(pt2);

		pt.push_back(std::make_pair("import", pt2));
	}

	// export
	pt.put("export", m_export->getName());

	ModuleModel::save_this(pt);

	pt.put("type", "SynthesizerModel");
}

void SynthesizerState::Update(CellState& cell)
{
	m_timer += g_systemManagerPtr->GetDeltaTime();

	auto model = dynamic_pointer_cast<SynthesizerModel>(GetPartConfig()->getModel());
	if (m_timer > 2.0 && cell.m_storage >= model->GetImport() && cell.m_model->GetMaterial().numMolecule(model->GetExport()) > cell.m_storage.numMolecule(model->GetExport()))
	{
		m_timer = 0.0;

		cell.m_storage -= model->GetImport();
		cell.m_storage.addMolecule(model->GetExport(), 1);
	}
}
