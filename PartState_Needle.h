#pragma once

#include "PartState.h"

class PartAsset_Needle;

class PartState_Needle
	: public PartState
{
	std::shared_ptr<PartAsset_Needle> m_partAsset_Needle;
	double	m_heat = 0.0;

public:
	PartState_Needle(const std::shared_ptr<PartConfig>& partConfig);

	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;

	void	load(Deserializer<ByteArray>& reader) override;
	void	save(Serializer<MemoryWriter>& writer) const override;
};
