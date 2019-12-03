#pragma once

#include "Rigidbody.h"

class CellAsset;

class EggState
	: public Rigidbody
{
	shared_ptr<CellAsset>	m_cellAsset;
	double	m_timer;

public:
	EggState(const shared_ptr<CellAsset>& cellAsset);
	EggState(const JSONValue& json) { load(json); }

	const shared_ptr<CellAsset>&	getCellAsset() const { return m_cellAsset; }

	void	updateEgg();
	void	draw();

	// JSON
	void	load(const JSONValue& json);
	void	save(JSONWriter& json) const;
};
