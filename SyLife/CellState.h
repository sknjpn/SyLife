#pragma once

#include "Rigidbody.h"

#include "Storage.h"

class CellAsset;
class PartState;
class ElementState;

class CellState
	: public Rigidbody
{
public:
	double	m_startTimer;
	double	m_deathTimer;
	double	m_yieldTimer;
	Storage	m_storage;

	shared_ptr<CellAsset>	m_model;

	Array<shared_ptr<PartState>>		m_partStates;

public:
	CellState(const shared_ptr<CellAsset>& model);

	void	updateCell();
	void	draw();

	void	takeNutrition();
	void	takeElement(const shared_ptr<ElementState>& element);
};
