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
	const Storage& getStorage() const { return m_storage; }
	double	getNutrition() const { return m_nutrition; }

	// JSON
	void	Load_this(const ptree& pt)
	{
		// storage
		m_storage.Load(pt.get_child("storage"));

		// nutrition
		m_nutrition = pt.get<double>("nutrition");

		Model::Load_this(pt);
	}
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const
	{
		// storage
		{
			ptree storage;

			m_storage.Save(storage);

			pt.add_child("storage", storage);
		}

		// nutrition
		pt.put<double>("nutrition", m_nutrition);

		Model::Save_this(pt);

		// type
		pt.put("type", "Storage");
	}
	void	Save(ptree& pt) const override { Save_this(pt); }
};

