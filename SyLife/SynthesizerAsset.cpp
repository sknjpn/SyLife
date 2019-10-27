#include "SynthesizerAsset.h"

#include "ViewerManager.h"
#include "AssetManager.h"

#include "SynthesizerEditor.h"
#include "SynthesizerState.h"
#include "ElementAsset.h"
#include "PartShapeViewer.h"

void SynthesizerAsset::makeViewer()
{
	g_viewerManagerPtr->makeViewer<SynthesizerEditor>()->setPartAsset(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setPartAsset(shared_from_this());
}

shared_ptr<PartState> SynthesizerAsset::makeState()
{
	return make_shared<SynthesizerState>();
}

void SynthesizerAsset::load_this(const ptree& pt)
{
	// export
	m_export = g_assetManagerPtr->getAsset<ElementAsset>(pt.get<string>("export"));

	ModuleAsset::load_this(pt);
}

void SynthesizerAsset::save_this(ptree& pt) const
{
	// export
	pt.put("export", m_export->getName());
	
	ModuleAsset::save_this(pt);

	pt.put("type", "SynthesizerAsset");
}
