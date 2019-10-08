#include "WingAsset.h"

#include "ViewerManager.h"

#include "WingState.h"
#include "WingEditor.h"
#include "PartShapeViewer.h"

void WingAsset::makeViewer()
{
	g_viewerManagerPtr->makeViewer<WingEditor>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> WingAsset::makeState()
{
	return make_shared<WingState>();
}

void WingAsset::load_this(const ptree& pt)
{
	m_isRight = pt.get<bool>("isRight");

	EquipmentAsset::load_this(pt);
}

void WingAsset::save_this(ptree& pt) const
{
	pt.put("isRight", m_isRight);

	EquipmentAsset::save_this(pt);

	pt.put("type", "WingAsset");
}
