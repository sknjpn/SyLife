#pragma once

#include "PartState.h"

class Part_PoisonerAsset;

class Part_PoisonerState
	: public PartState
{
	std::shared_ptr<Part_PoisonerAsset> m_Part_PoisonerAsset;
	double	m_heat = 0;

public:
	Part_PoisonerState(const std::shared_ptr<PartConfig>& partConfig);

	const std::shared_ptr<Part_PoisonerAsset>& getPart_PoisonerAsset() const { return m_Part_PoisonerAsset; }

	void	update(CellState& cellState) override;

	void	load(Deserializer<ByteArray>& reader) override;
	void	save(Serializer<MemoryWriter>& writer) const override;
};
