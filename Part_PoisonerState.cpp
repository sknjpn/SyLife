#include "Part_PoisonerState.h"
#include "Part_PoisonerAsset.h"
#include "PartConfig.h"
#include "CellState.h"
#include "World.h"

Part_PoisonerState::Part_PoisonerState(const std::shared_ptr<PartConfig>& partConfig)
	: PartState(partConfig)
	, m_Part_PoisonerAsset(std::dynamic_pointer_cast<Part_PoisonerAsset>(partConfig->getPartAsset()))
{}

void Part_PoisonerState::update(CellState& cellState)
{
	m_heat += DeltaTime;

	if (m_heat > m_Part_PoisonerAsset->getInterval())
	{
		m_heat = 0;

		World::GetInstance()->getTile(cellState.getPosition())
			.addPoison(m_Part_PoisonerAsset->getAmount());
	}
}

void Part_PoisonerState::load(Deserializer<ByteArray>& reader)
{
	reader >> m_heat;
}

void Part_PoisonerState::save(Serializer<MemoryWriter>& writer) const
{
	writer << m_heat;
}
