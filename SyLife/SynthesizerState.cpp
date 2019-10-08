#include "SynthesizerState.h"

#include "SystemManager.h"

#include "SynthesizerAsset.h"
#include "PartConfig.h"
#include "CellState.h"


void SynthesizerState::draw(const CellState& cellState) const
{
	getPartConfig()->getModel()->draw(min(m_timer / 2.0, 1.0) * 0.75 + 0.25);
}

void SynthesizerState::update(CellState& cellState)
{
	/*
	m_timer += g_systemManagerPtr->GetDeltaTime();

	auto model = dynamic_pointer_cast<SynthesizerAsset>(getPartConfig()->getModel());
	if (m_timer > 2.0 && cellState.m_storage >= model->GetImport() && cellState.m_model->getMaterial().numElement(model->GetExport()) > cellState.m_storage.numElement(model->GetExport()))
	{
		m_timer = 0.0;

		cellState.m_storage -= model->GetImport();
		cellState.m_storage.addElement(model->GetExport(), 1);
	}
	*/
}