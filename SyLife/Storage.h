#pragma once

#include "Model.h"
#include "Molecule.h"

class MoleculeModel;

class Storage
	: public Model
	, private Array<pair<shared_ptr<MoleculeModel>, int>>
{	
	double m_nutrition;

public:
	// operator
	Storage	operator+(const Storage& s) const { return Storage(*this) += s; }
	Storage	operator-(const Storage& s) const { return Storage(*this) -= s; }
	bool operator>=(const Storage& s) const;
	bool operator<=(const Storage& s) const;
	Storage& operator+=(const Storage& s) noexcept;
	Storage& operator-=(const Storage& s) noexcept;

	// molecule
	const Array<pair<shared_ptr<MoleculeModel>, int>>&	getMoleculeList() const { return *this; }
	void	addMolecule(const shared_ptr<MoleculeModel>& model, int size);
	void	pullMolecule(const shared_ptr<MoleculeModel>& model, int size);
	int		numMolecule(const shared_ptr<MoleculeModel>& model) const;

	// JSON
	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
};
