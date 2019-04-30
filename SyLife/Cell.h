#pragma once

#include "Rigidbody.h"
#include "Storage.h"
#include "Nucleus.h"
#include "Molecule.h"

class Part;

class Cell : public Rigidbody
{
public:
	Nucleus m_nucleus;
	Storage	m_storage;
	vector<shared_ptr<Part>>	m_part;

public:
	Cell();
	~Cell();

	void	Update();
	void	Draw();

	void	ExpireMolecule(const shared_ptr<Molecule::Model>& model) const;
	void	ExpireMolecule(const shared_ptr<Molecule::Model>& model, int size) const;
};

