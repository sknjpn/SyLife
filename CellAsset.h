﻿#pragma once

#include "Asset.h"
#include "Storage.h"

class PartConfig;
class PartAsset_Body;
class PartAsset_Nucleus;

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
	double	m_drawRadius;

	// timer
	double	m_lifespanTime;	// 寿命
	double	m_bornTime;		// 生まれるまでの時間
	double	m_yieldTime;	// 生むのにかかる時間

	double	m_maxHitPoint;

	void	updateMass();
	void	updateRadius() { m_radius = sqrt(2 * m_inertia / m_mass); }
	void	updateDrawRadius();
	void	updateInertia();
	void	updateMaxStorage();
	void	updateMaterial();

public:
	bool	isValid() const;

	Vec2	getCentroid();

	void	setCentroidAsOrigin();

	void	removePartConfig(const shared_ptr<PartConfig>& partConfig) { m_partConfigs.remove(partConfig); }

	// Get
	const Storage& getMaterial() const { return m_material; }
	const Storage& getMaxStorage() const { return m_maxStorage; }
	double	getMass() const { return m_mass; }
	double	getRadius() const { return m_radius; }
	double	getDrawRadius() const { return m_drawRadius; }
	double	getInertia() const { return m_inertia; }
	const Array<shared_ptr<PartConfig>>& getPartConfigs() const { return m_partConfigs; }
	double	getLifespanTime() const { return m_lifespanTime; }
	double	getBornTime() const { return m_bornTime; }
	double	getYieldTime() const { return m_yieldTime; }
	double	getMaxHitPoint() const { return m_maxHitPoint; }

	// Add
	shared_ptr<PartConfig>& addPartConfig();

	void	updateProperties();

	shared_ptr<PartAsset_Body> getBodyAsset() const;
	shared_ptr<PartAsset_Nucleus> getNucleusAsset() const;

	// draw
	void	draw(double a = 0.5);

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;

	// TypeName
	String	getTypeName() override { return U"CellAsset"; }

	class Log
	{
	public:
		class Status
		{
		public:
			int	m_numCell;
			int m_numEgg;

			Status(int numCell, int numEgg)
				: m_numCell(numCell)
				, m_numEgg(numEgg)
			{}
		};

		Array<Status> m_statuses;

		void update(const shared_ptr<CellAsset>& cellAsset);
	} m_log;
};
