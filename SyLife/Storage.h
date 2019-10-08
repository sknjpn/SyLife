#pragma once

#include "Model.h"

class ElementAsset;

class Storage
	: public Model
	, private Array<pair<shared_ptr<ElementAsset>, int>>
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

	double	getNutrition() const { return m_nutrition; }

	// element
	const Array<pair<shared_ptr<ElementAsset>, int>>&	getElementList() const { return *this; }
	void	addElement(const shared_ptr<ElementAsset>& model, int size = 1);
	void	pullElement(const shared_ptr<ElementAsset>& model, int size = 1);
	int		numElement(const shared_ptr<ElementAsset>& model) const;

	// JSON
	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
};
