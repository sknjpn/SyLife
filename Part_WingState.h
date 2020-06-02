#pragma once

#include "PartState.h"

class Part_WingAsset;

class Part_WingState
	: public PartState
{
	std::shared_ptr<Part_WingAsset> m_Part_WingAsset;
	int		m_timer = 0;
	double	m_v = 0.0;
	double	m_p = 0.0;
	int		m_counter = 0;

public:
	Part_WingState(const std::shared_ptr<PartConfig>& partConfig);

	const std::shared_ptr<Part_WingAsset>& getPart_WingAsset() const { return m_Part_WingAsset; }

	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;

	void	flap(CellState& cellState);

	void	stop() { m_counter = 1; }
	void	move() { m_counter = 0; m_timer = Max(m_timer, 30); }

	void	load(Deserializer<ByteArray>& reader) override;
	void	save(Serializer<MemoryWriter>& writer) const override;
};
