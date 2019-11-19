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

void SynthesizerAsset::load(const JSONValue& json)
{
	ModuleAsset::load(json);

	// export
	m_export = g_assetManagerPtr->getAsset<ElementAsset>(json[U"export"].getString());
}

void SynthesizerAsset::save(const JSONWriter& json) const
{
	ModuleAsset::save(json);

	// export
	//pt.put(U"export", m_export->getName());

	//pt.put(U"type", "SynthesizerAsset");
}
