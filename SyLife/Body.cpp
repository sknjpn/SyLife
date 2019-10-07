﻿#include "Body.h"

#include "ViewerManager.h"

void BodyModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<BodyViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}

shared_ptr<PartState> BodyModel::MakeState()
{
	return make_shared<BodyState>();
}
