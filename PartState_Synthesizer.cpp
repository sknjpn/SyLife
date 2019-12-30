#include "PartState_Synthesizer.h"



#include "PartAsset_Synthesizer.h"
#include "PartConfig.h"
#include "CellState.h"
#include "CellAsset.h"
#include "ProteinAsset.h"


void PartState_Synthesizer::draw(const CellState& cellState) const
{
	getPartConfig()->getPartAsset()->draw(min(m_timer / 2.0, 1.0) * 0.75 + 0.25);
}

void PartState_Synthesizer::update(CellState& cellState)
{
	m_timer += DeltaTime;

	auto asset = dynamic_pointer_cast<PartAsset_Synthesizer>(getPartConfig()->getPartAsset());
	if (m_timer > asset->getProductTime() &&
		cellState.m_storage >= asset->getExport()->getMaterial() &&
		cellState.m_cellAsset->getMaxStorage().numProtein(asset->getExport()) > cellState.m_storage.numProtein(asset->getExport()))
	{
		m_timer = 0.0;

		cellState.m_storage -= asset->getExport()->getMaterial();
		cellState.m_storage.addProtein(asset->getExport());
	}
}