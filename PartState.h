#pragma once

#include "common.h"

class PartConfig;
class CellState;

class PartState
{
	shared_ptr<PartConfig>	m_partConfig;

public:
	PartState(const shared_ptr<PartConfig>& partConfig)
		: m_partConfig(partConfig)
	{}
	virtual ~PartState() {}

	// Get
	const shared_ptr<PartConfig>& getPartConfig() const { return m_partConfig; }

	virtual void	draw(const CellState& cellState) const;
	virtual void	update(CellState& cellState);

	virtual void	load(Deserializer<ByteArray>& reader) {}
	virtual void	save(Serializer<MemoryWriter>& writer) const {}
};
