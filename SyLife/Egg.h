#pragma once

#include "Rigidbody.h"
#include "Cell.h"

class EggState
	: public Rigidbody
{
	shared_ptr<CellModel>	m_cellModel;
	double	m_timer;

public:
	EggState() { m_timer = 10.0; }

	void	SetCellModel(const shared_ptr<CellModel>& cellModel)
	{
		m_cellModel = cellModel;
		m_radius = cellModel->m_radius / 2.0;
		m_mass = cellModel->m_mass / 4.0;
		m_inertia = m_mass * m_radius * m_radius / 2.0;
	}

	const shared_ptr<CellModel>&	GetCellModel() const { return m_cellModel; }

	void	Update();
	void	Draw();
};

