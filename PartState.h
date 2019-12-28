#pragma once

class PartConfig;
class CellState;

class PartState
{
	shared_ptr<PartConfig>	m_partConfig;

public:
	virtual ~PartState() {}

	// Get
	const shared_ptr<PartConfig>& getPartConfig() const { return m_partConfig; }

	// Set
	void	setPartConfig(const shared_ptr<PartConfig>& partConfig) { m_partConfig = partConfig; }

	virtual void	draw(const CellState& cellState) const;
	virtual void	update(CellState& cellState);

	virtual void	load(Deserializer<ByteArray>& reader) {}
	virtual void	save(Serializer<MemoryWriter>& writer) const {}
};
