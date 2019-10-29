#include "SynthesizerState.h"



#include "SynthesizerAsset.h"
#include "PartConfig.h"
#include "CellState.h"
#include "CellAsset.h"
#include "ElementAsset.h"


void SynthesizerState::draw(const CellState& cellState) const
{
	getPartConfig()->getPartAsset()->draw(min(m_timer / 2.0, 1.0) * 0.75 + 0.25);
}

void SynthesizerState::update(CellState& cellState)
{
	m_timer += DeltaTime;

	auto asset = dynamic_pointer_cast<SynthesizerAsset>(getPartConfig()->getPartAsset());
	if (m_timer > 2.0 &&
		cellState.m_storage >= asset->getExport()->getMaterial() &&
		cellState.m_cellAsset->getMaxStorage().numElement(asset->getExport()) > cellState.m_storage.numElement(asset->getExport()))
	{
		m_timer = 0.0;

		cellState.m_storage -= asset->getExport()->getMaterial();
		cellState.m_storage.addElement(asset->getExport());
	}
}