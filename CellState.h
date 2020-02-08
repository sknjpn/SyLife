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
	double	m_bioaccumulation;	// 毒の蓄積

	Storage	m_storage;

	std::shared_ptr<CellAsset>	m_cellAsset;

	Array<std::shared_ptr<PartState>>		m_partStates;

public:
	CellState(const std::shared_ptr<CellAsset>& cellAsset);
	CellState(Deserializer<ByteArray>& reader) { load(reader); }

	const std::shared_ptr<CellAsset>& getCellAsset() const { return m_cellAsset; }

	void	updateCell();
	void	draw();

	void	takeElement();
	bool	isNeedNutrition() const;

	void	load(Deserializer<ByteArray>& reader);
	void	save(Serializer<MemoryWriter>& writer) const;
};
