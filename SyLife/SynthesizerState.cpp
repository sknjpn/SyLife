#include "SynthesizerState.h"

#include "SystemManager.h"

#include "SynthesizerAsset.h"
#include "PartConfig.h"
#include "CellState.h"
#include "CellAsset.h"
#include "ElementAsset.h"


void SynthesizerState::draw(const CellState& cellState) const
{
	getPartConfig()->getModel()->draw(min(m_timer / 2.0, 1.0) * 0.75 + 0.25);
}

void SynthesizerState::update(CellState& cellState)
{
	m_timer += g_systemManagerPtr->GetDeltaTime();

	auto model = dynamic_pointer_cast<SynthesizerAsset>(getPartConfig()->getModel());
	if (m_timer > 2.0 &&
		cellState.m_storage >= model->getExport()->getMaterial() &&
		cellState.m_model->getMaxStorage().numElement(model->getExport()) > cellState.m_storage.numElement(model->getExport()))
	{
		m_timer = 0.0;

		cellState.m_storage -= model->getExport()->getMaterial();
		cellState.m_storage.addElement(model->getExport());
	}
}