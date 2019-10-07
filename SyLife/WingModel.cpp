#include "WingModel.h"
#include "WingState.h"
#include "WingEditor.h"

#include "ViewerManager.h"

void WingModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<WingEditor>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> WingModel::MakeState()
{
	return make_shared<WingState>();
}

void WingModel::load_this(const ptree& pt)
{
	m_isRight = pt.get<bool>("isRight");

	EquipmentModel::load_this(pt);
}
