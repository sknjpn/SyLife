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

	const std::shared_ptr<PartAsset_Wing>& getPartAsset_Wing() const { return m_partAsset_Wing; }

	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;

	void	flap(CellState& cellState);

	void	stop() { m_counter = 1; }
	void	move() { m_counter = 0; m_timer = Max(m_timer, 30); }

	void	load(Deserializer<ByteArray>& reader) override;
	void	save(Serializer<MemoryWriter>& writer) const override;
};
