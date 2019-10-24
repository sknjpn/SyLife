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

	const shared_ptr<CellAsset>&	getCellAsset() const { return m_cellAsset; }

	void	updateEgg();
	void	draw();
};
