#pragma once

#include "Model.h"

class ElementAsset;

class Storage
	: public Model
	, private Array<pair<shared_ptr<ElementAsset>, int>>
{
	double m_nutrition;

public:
	Storage()
		: m_nutrition(0.0)
	{}

	// operator
	Storage	operator+(const Storage& s) const { return Storage(*this) += s; }
	Storage	operator-(const Storage& s) const { return Storage(*this) -= s; }
	bool operator>=(const Storage& s) const;
	bool operator<=(const Storage& s) const;
	Storage& operator+=(const Storage& s) noexcept;
	Storage& operator-=(const Storage& s) noexcept;

	bool	contain(const Storage& s) const;

	double	getNutrition() const { return m_nutrition; }
	void	setNutrition(double nutrition) { m_nutrition = nutrition; }
	void	addNutrition(double nutrition) { m_nutrition += nutrition; }
	void	pullNutrition(double nutrition) { m_nutrition -= nutrition; }

	// element
	const Array<pair<shared_ptr<ElementAsset>, int>>& getElementList() const { return *this; }
	void	addElement(const shared_ptr<ElementAsset>& asset, int size = 1);
	void	pullElement(const shared_ptr<ElementAsset>& asset, int size = 1);
	int		numElement(const shared_ptr<ElementAsset>& asset) const;

	// JSON
	void	load(const ptree& pt) override;
	void	save(ptree& pt) const override;
};
