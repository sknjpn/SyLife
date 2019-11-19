#pragma once

#include "Asset.h"

#include "Storage.h"

class PartConfig;
class BodyAsset;
class NucleusAsset;

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

	void	updateMass();
	void	updateRadius() { m_radius = sqrt(2 * m_inertia / m_mass); }
	void	updateInertia();
	void	updateMaxStorage();
	void	updateMaterial();

public:
	bool	isValid() const;

	void	makeViewer() override;

	Vec2	getCentroid();

	void	setCentroidAsOrigin();

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

	shared_ptr<BodyAsset> getBodyAsset() const;
	shared_ptr<NucleusAsset> getNucleusAsset() const;

	// draw
	void	draw(double a = 0.5);

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
};
