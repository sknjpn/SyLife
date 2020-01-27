#pragma once

#include "PartState.h"

class PartAsset_Wing;

class PartState_Wing
	: public PartState
{
	std::shared_ptr<PartAsset_Wing> m_partAsset_Wing;
	int		m_timer = 0;
	double	m_v = 0.0;
	double	m_p = 0.0;
	int		m_counter = 0;

public:
	PartState_Wing(const std::shared_ptr<PartConfig>& partConfig);

	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;

	void	flap(CellState& cellState);

	void	load(Deserializer<ByteArray>& reader) override;
	void	save(Serializer<MemoryWriter>& writer) const override;
};
