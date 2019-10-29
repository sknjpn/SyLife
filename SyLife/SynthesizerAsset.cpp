#include "SynthesizerAsset.h"

#include "AssetManager.h"

#include "SynthesizerEditor.h"
#include "SynthesizerState.h"
#include "ElementAsset.h"
#include "PartShapeViewer.h"

void SynthesizerAsset::makeViewer()
{
	//g_viewerManagerPtr->makeViewer<SynthesizerEditor>()->setPartAsset(shared_from_this());
	//g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setPartAsset(shared_from_this());
}

shared_ptr<PartState> SynthesizerAsset::makeState()
{
	return make_shared<SynthesizerState>();
}

void SynthesizerAsset::load(const ptree& pt)
{
	ModuleAsset::load(pt);

	// export
	m_export = g_assetManagerPtr->getAsset<ElementAsset>(pt.get<string>("export"));
}

void SynthesizerAsset::save(ptree& pt) const
{
	ModuleAsset::save(pt);

	// export
	pt.put("export", m_export->getName());

	pt.put("type", "SynthesizerAsset");
}
