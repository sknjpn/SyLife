#pragma once

#include "Rigidbody.h"


class Molecule : public Rigidbody
{
public:
	shared_ptr<MoleculeModel>	m_model;
	
public:
};
