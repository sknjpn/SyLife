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
	Storage	m_storage;

	shared_ptr<CellAsset>	m_model;

	Array<shared_ptr<PartState>>		m_partStates;

public:
	CellState(const shared_ptr<CellAsset>& model);

	void	UpdateCell();
	void	draw();

	// void	TakeMolecule(const shared_ptr<MoleculeState>& molecule);
	// void	ExpireMolecule(const shared_ptr<MoleculeAsset>& model, unsigned int size = 1);
};
