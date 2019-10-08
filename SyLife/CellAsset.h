#pragma once

#include "Asset.h"

#include "Storage.h"

class PartConfig;

class CellAsset
	: public Asset
{
	Storage	m_material;
	Storage	m_maxStorage;
	Array<shared_ptr<PartConfig>>	m_partConfigs;

	// Rigidbody
	double	m_mass;
	double	m_radius;
	double	m_inertia;

public:
	void	makeViewer() override;

	// Get
	const Storage& getMaterial() const { return m_material; }
	double	getMass() const { return m_mass; }
	double	getRadius() const { return m_radius; }
	double	getInertia() const { return m_inertia; }
	const Array<shared_ptr<PartConfig>>& getPartConfigs() const { return m_partConfigs; }
	Array<shared_ptr<PartConfig>>& getPartConfigs() { return m_partConfigs; }

	// Add
	shared_ptr<PartConfig>& addPartConfig();

	template <typename T>
	Array<shared_ptr<T>>	getPartConfigs() const
	{
		Array<shared_ptr<T>> tModels;

		for (auto it = m_partConfigs.begin(); it != m_partConfigs.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) tModels.emplace_back(dynamic_pointer_cast<T>(*it));

		return tModels;
	}

	void	updateProperties();

	// draw
	void	draw(double a = 0.5);

	// JSON
	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
};