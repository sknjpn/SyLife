#include "NeedleState.h"
#include "NeedleModel.h"

#include "PartConfig.h"

#include "Cell.h"

#include "SystemManager.h"
#include "CellManager.h"

void NeedleState::draw(const CellState& cell) const
{
	auto t = Transformer2D(Mat3x2::Scale(1.0, max(m_heat - 4.0, 0.0) * 1.0 + 1.0));

	getPartConfig()->getModel()->draw(max(m_heat - 4.0, 0.0) * 0.9 + 0.1);
}

void NeedleState::update(CellState& cell)
{
	m_heat -= g_systemManagerPtr->GetDeltaTime();
	if (m_heat < 0)
	{
		m_heat = 5.0;

		auto p = cell.GetWorldPosition(getPartConfig()->GetPosition() + Vec2::Up() * 50.0);


		for (auto i : g_cellManagerPtr->GetCellStateKDTree().knnSearch(1, p))
		{
			auto& t = g_cellManagerPtr->GetCellStates()[i];

			if (!t->IsDestroyed() && t->getRadius() > (t->GetPosition() - p).length() && t->m_model != cell.m_model)
			{
				t->Destroy();
				cell.m_storage += t->m_storage;
				cell.m_storage += t->m_model->getMaterial();
			}
		}
	}
}