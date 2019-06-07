#pragma once

#include "Molecule.h"

class Recipe
{
	string	m_name;
	vector<pair<shared_ptr<MoleculeModel>, int>>	m_material;
	vector<pair<shared_ptr<MoleculeModel>, int>>	m_product;

public:
};

