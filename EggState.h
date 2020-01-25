#pragma once

#include "Rigidbody.h"

class CellAsset;

class EggState
	: public Rigidbody
{
	std::shared_ptr<CellAsset>	m_cellAsset;
	double	m_timer;

public:
	EggState(const std::shared_ptr<CellAsset>& cellAsset);
	EggState(Deserializer<ByteArray>& reader) { load(reader); }

	const std::shared_ptr<CellAsset>&	getCellAsset() const { return m_cellAsset; }

	void	updateEgg();
	void	draw();

	void	load(Deserializer<ByteArray>& reader);
	void	save(Serializer<MemoryWriter>& writer) const;
};
