﻿#include "Wing.h"

#include "ViewerManager.h"

void WingState::Flap(CellState& cell)
{
	cell.AddImpulseInLocal(Vec2::Up() * 10000.0, GetPartConfig()->GetPosition());
}

void WingModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<WingViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
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