#include "CellAsset.h"
#include "CellState.h"
#include "EggState.h"
#include "World.h"

void CellAsset::Log::update(const std::shared_ptr<CellAsset>& cellAsset)
{
	const int numCell = int(World::GetInstance()->getCellStates().count_if([&cellAsset](const auto& cs) { return cs->getCellAsset() == cellAsset; }));
	const int numEgg = int(World::GetInstance()->getEggStates().count_if([&cellAsset](const auto& es) { return es->getCellAsset() == cellAsset; }));

	m_statuses.emplace_back(numCell, numEgg);
}
