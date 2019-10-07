#pragma once

#include "Rigidbody.h"

class CellModel;

class EggState
	: public Rigidbody
{
	shared_ptr<CellModel>	m_cellModel;
	double	m_timer;

public:
	EggState() { m_timer = 10.0; }

	void	SetCellModel(const shared_ptr<CellModel>& cellModel);

	const shared_ptr<CellModel>&	getCellModel() const { return m_cellModel; }

	void	updateEgg();
	void	draw();
};
