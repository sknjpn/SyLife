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

	// timer
	double	m_lifespanTime;	// 寿命
	double	m_bornTime;		// 生まれるまでの時間
	double	m_yieldTime;	// 生むのにかかる時間

	Vec2	getCenter();
	void	updateRadius() { m_radius = sqrt(2 * m_inertia / m_mass); }
	void	updateInertia();
	void	updateMaxStorage();
	void	updateMaterial();

public:
	void	makeViewer() override;

	// Get
	const Storage& getMaterial() const { return m_material; }
	const Storage& getMaxStorage() const { return m_maxStorage; }
	double	getMass() const { return m_mass; }
	double	getRadius() const { return m_radius; }
	double	getInertia() const { return m_inertia; }
	const Array<shared_ptr<PartConfig>>& getPartConfigs() const { return m_partConfigs; }
	double	getLifespanTime() const { return m_lifespanTime; }
	double	getBornTime() const { return m_bornTime; }
	double	getYieldTime() const { return m_yieldTime; }

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