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

void WingAsset::load(const ptree& pt)
{
	EquipmentAsset::load(pt);

	m_isRight = pt.get<bool>("isRight");
}

void WingAsset::save(ptree& pt) const
{
	EquipmentAsset::save(pt);

	pt.put("isRight", m_isRight);

	pt.put("type", "WingAsset");
}
