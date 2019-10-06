#pragma once

#include "Model.h"
#include "Storage.h"

// あるものを生産するのに必要な量
class Material
	: public Model
{
	Storage m_storage;
	double	m_nutrition;

public:
	Material(const Storage& storage, double nutrition)
		: m_storage(storage)
		, m_nutrition(nutrition)
	{}

	const Storage& getStorage() const { return m_storage; }
	double	getNutrition() const { return m_nutrition; }

	// operator
	Material	operator +(const Material& s) const { return Material(*this) += s; }
	Material	operator -(const Material& s) const { return Material(*this) -= s; }
	bool operator >=(const Material& s) const
	{
		return m_storage >= s.m_storage && m_nutrition >= s.m_nutrition;
	}
	bool operator <=(const Material& s) const
	{
		return m_storage <= s.m_storage && m_nutrition <= s.m_nutrition;
	}
	Material& operator +=(const Material& s) noexcept
	{
		m_storage += s.m_storage;
		m_nutrition += s.m_nutrition;

		return *this;
	}
	Material& operator -=(const Material& s) noexcept
	{
		m_storage -= s.m_storage;
		m_nutrition -= s.m_nutrition;

		return *this;
	}

	void	claer() { m_storage.reset(); m_nutrition = 0.0; }
	bool	isEmpty() const { return m_storage.isEmpty() && m_nutrition == 0.0; }

	// JSON
	void	load_this(const ptree& pt)
	{
		// storage
		m_storage.load(pt.get_child("storage"));

		// nutrition
		m_nutrition = pt.get<double>("nutrition");

		Model::load_this(pt);
	}
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const
	{
		// storage
		{
			ptree storage;

			m_storage.save(storage);

			pt.add_child("storage", storage);
		}

		// nutrition
		pt.put<double>("nutrition", m_nutrition);

		Model::save_this(pt);

		// type
		pt.put("type", "Storage");
	}
	void	save(ptree& pt) const override { save_this(pt); }
};

