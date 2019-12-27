#pragma once

#include "Model.h"
#include "Viewer.h"

class ProteinAsset;

class Storage
	: public Model
	, private Array<pair<shared_ptr<ProteinAsset>, int>>
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

	double	getNutritionRecursive() const;

	// protein
	const Array<pair<shared_ptr<ProteinAsset>, int>>& getProteinList() const { return *this; }
	void	addProtein(const shared_ptr<ProteinAsset>& asset, int size = 1);
	void	pullProtein(const shared_ptr<ProteinAsset>& asset, int size = 1);
	int		numProtein(const shared_ptr<ProteinAsset>& asset) const;

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	void	load(Deserializer<ByteArray>& reader);
	void	save(Serializer<MemoryWriter>& writer) const;
};
