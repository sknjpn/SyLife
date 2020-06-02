#pragma once

#include "PartState.h"

class Part_SynthesizerAsset;

class Part_SynthesizerState
	: public PartState
{
	std::shared_ptr<Part_SynthesizerAsset> m_Part_SynthesizerAsset;
	double	m_timer = 0.0;

public:
	Part_SynthesizerState(const std::shared_ptr<PartConfig>& partConfig);

	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;

	void	load(Deserializer<ByteArray>& reader) override;
	void	save(Serializer<MemoryWriter>& writer) const override;
};
