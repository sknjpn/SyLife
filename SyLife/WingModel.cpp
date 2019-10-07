#include "WingModel.h"
#include "WingState.h"
#include "WingEditor.h"

#include "ViewerManager.h"

void WingModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<WingEditor>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> WingModel::makeState()
{
	return make_shared<WingState>();
}

void WingModel::load_this(const ptree& pt)
{
	m_isRight = pt.get<bool>("isRight");

	EquipmentModel::load_this(pt);
}

void WingModel::save_this(ptree& pt) const
{
	pt.put("isRight", m_isRight);

	EquipmentModel::save_this(pt);

	pt.put("type", "WingModel");
}
