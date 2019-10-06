#include "Chloroplast.h"

#include "ViewerManager.h"

void ChloroplastState::Update(CellState& cell)
{
}

void ChloroplastState::MakeNutrition()
{
}

void ChloroplastModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<ChloroplastViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}

shared_ptr<PartState> ChloroplastModel::MakeState()
{
	return make_shared<ChloroplastState>();
}
