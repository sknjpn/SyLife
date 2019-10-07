#include "SynthesizerState.h"
#include "SynthesizerModel.h"

#include "PartConfig.h"

#include "Cell.h"

#include "SystemManager.h"

void SynthesizerState::Update(CellState& cell)
{
	m_timer += g_systemManagerPtr->GetDeltaTime();

	auto model = dynamic_pointer_cast<SynthesizerModel>(GetPartConfig()->getModel());
	if (m_timer > 2.0 && cell.m_storage >= model->GetImport() && cell.m_model->GetMaterial().numMolecule(model->GetExport()) > cell.m_storage.numMolecule(model->GetExport()))
	{
		m_timer = 0.0;

		cell.m_storage -= model->GetImport();
		cell.m_storage.addMolecule(model->GetExport(), 1);
	}
}