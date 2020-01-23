#pragma once

#include "Rigidbody.h"

#include "Storage.h"

class CellAsset;
class PartState;

class CellState
	: public Rigidbody
{
public:
	double	m_startTimer;
	double	m_deathTimer;
	double	m_yieldTimer;

	Storage	m_storage;

	shared_ptr<CellAsset>	m_cellAsset;

	Array<shared_ptr<PartState>>		m_partStates;

public:
	CellState(const shared_ptr<CellAsset>& cellAsset);
	CellState(Deserializer<ByteArray>& reader) { load(reader); }

	const shared_ptr<CellAsset>& getCellAsset() const { return m_cellAsset; }

	void	updateCell();
	void	draw();

	void	takeElement();

	void	load(Deserializer<ByteArray>& reader);
	void	save(Serializer<MemoryWriter>& writer) const;
};
