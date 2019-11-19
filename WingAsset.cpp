#include "WingAsset.h"

#include "WingState.h"
#include "WingEditor.h"
#include "PartShapeViewer.h"

void WingAsset::makeViewer()
{
	//g_viewerManagerPtr->makeViewer<WingEditor>()->setPartAsset(shared_from_this());
	//g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setPartAsset(shared_from_this());
}

shared_ptr<PartState> WingAsset::makeState()
{
	return make_shared<WingState>();
}

void WingAsset::load(const JSONValue& json)
{
	EquipmentAsset::load(json);

	m_isRight = json[U"isRight"].get<bool>();
}

void WingAsset::save(const JSONWriter& json) const
{
	EquipmentAsset::save(json);

	//pt.put(U"isRight", m_isRight);

	//pt.put(U"type", "WingAsset");
}
