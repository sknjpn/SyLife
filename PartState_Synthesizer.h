﻿#pragma once

#include "PartState.h"

class PartAsset_Synthesizer;

class PartState_Synthesizer
	: public PartState
{
	std::shared_ptr<PartAsset_Synthesizer> m_partAsset_Synthesizer;
	double	m_timer = 0.0;

public:
	PartState_Synthesizer(const std::shared_ptr<PartConfig>& partConfig);

	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;

	void	load(Deserializer<ByteArray>& reader) override;
	void	save(Serializer<MemoryWriter>& writer) const override;
};
