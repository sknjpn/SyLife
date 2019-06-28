#pragma once

#include "Particle.h"

class CellModel;

class EggState
	: public Particle
{
public:
	shared_ptr<CellModel>	m_cellModel;

public:
	EggState() {}
	~EggState() {}
};

