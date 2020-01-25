#include "PartState_Synthesizer.h"



#include "PartAsset_Synthesizer.h"
#include "PartConfig.h"
#include "CellState.h"
#include "CellAsset.h"
#include "ProteinAsset.h"


PartState_Synthesizer::PartState_Synthesizer(const std::shared_ptr<PartConfig>& partConfig)
	: PartState(partConfig)
	, m_partAsset_Synthesizer(dynamic_pointer_cast<PartAsset_Synthesizer>(partConfig->getPartAsset()))
{}

void PartState_Synthesizer::draw(const CellState& cellState) const
{
	getPartConfig()->getPartAsset()->draw(Min(m_timer / 2.0, 1.0) * 0.75 + 0.25);
}

void PartState_Synthesizer::update(CellState& cellState)
{
	m_timer += DeltaTime;

	if (m_timer > m_partAsset_Synthesizer->getProductTime() &&
		cellState.m_storage >= m_partAsset_Synthesizer->getExport()->getMaterial() &&
		cellState.m_cellAsset->getMaxStorage().numProtein(m_partAsset_Synthesizer->getExport()) > cellState.m_storage.numProtein(m_partAsset_Synthesizer->getExport()))
	{
		m_timer = 0.0;

		cellState.m_storage -= m_partAsset_Synthesizer->getExport()->getMaterial();
		cellState.m_storage.addProtein(m_partAsset_Synthesizer->getExport());
	}
}

void PartState_Synthesizer::load(Deserializer<ByteArray>& reader)
{
	reader >> m_timer;
}

void PartState_Synthesizer::save(Serializer<MemoryWriter>& writer) const
{
	writer << m_timer;
}
