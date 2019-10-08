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

	// void	TakeElement(const shared_ptr<ElementState>& element);
	// void	ExpireElement(const shared_ptr<ElementAsset>& model, unsigned int size = 1);
};
