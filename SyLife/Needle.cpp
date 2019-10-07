#include "Needle.h"

#include "SystemManager.h"
#include "ViewerManager.h"
#include "CellManager.h"

void NeedleModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<NeedleViewer>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> NeedleModel::MakeState()
{
	return make_shared<NeedleState>();
}

void NeedleState::Draw(const CellState& cell) const
{
	auto t = Transformer2D(Mat3x2::Scale(1.0, max(m_heat - 4.0, 0.0) * 1.0 + 1.0));

	GetPartConfig()->getModel()->Draw(max(m_heat - 4.0, 0.0) * 0.9 + 0.1);
}

void NeedleState::Update(CellState& cell)
{
	m_heat -= g_systemManagerPtr->GetDeltaTime();
	if (m_heat < 0)
	{
		m_heat = 5.0;

		auto p = cell.GetWorldPosition(GetPartConfig()->GetPosition() + Vec2::Up() * 50.0);


		for (auto i : g_cellManagerPtr->GetCellStateKDTree().knnSearch(1, p))
		{
			auto& t = g_cellManagerPtr->GetCellStates()[i];

			if (!t->IsDestroyed() && t->getRadius() > (t->GetPosition() - p).length() && t->m_model != cell.m_model)
			{
				t->Destroy();
				cell.m_storage += t->m_storage;
				cell.m_storage += t->m_model->GetMaterial();
			}
		}
	}
}