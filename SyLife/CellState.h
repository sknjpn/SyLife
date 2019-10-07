#pragma once

#include "Rigidbody.h"

#include "Storage.h"

class CellModel;
class PartState;

class CellState
	: public Rigidbody
{
public:
	double	m_startTimer;
	double	m_deathTimer;
	Storage	m_storage;

	shared_ptr<CellModel>	m_model;

	Array<shared_ptr<PartState>>		m_partStates;

public:
	CellState(const shared_ptr<CellModel>& model);

	void	UpdateCell();
	void	draw();

	// void	TakeMolecule(const shared_ptr<MoleculeState>& molecule);
	// void	ExpireMolecule(const shared_ptr<MoleculeModel>& model, unsigned int size = 1);
};
