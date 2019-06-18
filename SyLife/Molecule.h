#pragma once

#include "Rigidbody.h"

#include "MoleculeModel.h"

class Molecule
	: public Rigidbody
{
public:
	shared_ptr<MoleculeModel>	m_model;
	
public:
};
