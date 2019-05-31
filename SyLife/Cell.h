#pragma once

#include "Rigidbody.h"
#include "Storage.h"
#include "Molecule.h"

class Part;

class Cell : public Rigidbody
{
public:
	double	m_deathTimer;
	Storage	m_storage;
	Storage	m_molecules;
	vector<shared_ptr<Part>>	m_parts;

public:
	Cell();
	~Cell();

	void	RecalculatePhysicalProperty();
	void	Update();
	void	Draw();

	// FieldからStorageに受け取る
	void	TakeMolecule(const shared_ptr<Molecule>& molecule);

	// StorageからFieldに吐き出す
	void	ExpireMolecule(const shared_ptr<Molecule::Model>& model);
	void	ExpireMolecule(const shared_ptr<Molecule::Model>& model, int size);
};

