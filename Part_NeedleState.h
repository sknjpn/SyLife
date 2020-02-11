#pragma once

#include "PartState.h"

class Part_NeedleAsset;

class PartState_Needle
	: public PartState
{
	std::shared_ptr<Part_NeedleAsset> m_Part_NeedleAsset;
	double	m_heat = 0.0;

public:
	PartState_Needle(const std::shared_ptr<PartConfig>& partConfig);

	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;

	void	load(Deserializer<ByteArray>& reader) override;
	void	save(Serializer<MemoryWriter>& writer) const override;
};
