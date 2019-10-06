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

		for (auto m : pt.get_child("molecules"))
		{
			auto name = m.second.get<string>("molecule");

			const auto& model = g_assetManagerPtr->GetModel<MoleculeModel>(name);

			m_molecules.emplace_back(model, m.second.get<double>("size"));
		}

		// nutrition

		Model::Load_this(pt);
	}
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const
	{
		Model::Save_this(pt);

		// type
		pt.put("type", "Storage");
	}
	void	Save(ptree& pt) const override { Save_this(pt); }
};

