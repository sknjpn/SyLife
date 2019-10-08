#include "SynthesizerAsset.h"

#include "ViewerManager.h"
#include "AssetManager.h"
#include "SystemManager.h"

#include "SynthesizerEditor.h"
#include "SynthesizerState.h"
#include "ElementAsset.h"
#include "PartShapeViewer.h"

void SynthesizerAsset::makeViewer()
{
	g_viewerManagerPtr->makeViewer<SynthesizerEditor>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> SynthesizerAsset::makeState()
{
	return make_shared<SynthesizerState>();
}

void SynthesizerAsset::load_this(const ptree& pt)
{
	// import
	m_import.load(pt.get_child("import"));

	// export
	m_export = g_assetManagerPtr->getModel<ElementAsset>(pt.get<string>("export"));

	ModuleAsset::load_this(pt);
}

void SynthesizerAsset::save_this(ptree& pt) const
{
	// import
	{
		ptree pt2;

		m_import.save(pt2);

		pt.push_back(std::make_pair("import", pt2));
	}

	// export
	pt.put("export", m_export->getName());

	ModuleAsset::save_this(pt);

	pt.put("type", "SynthesizerAsset");
}
