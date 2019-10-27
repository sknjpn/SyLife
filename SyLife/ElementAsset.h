#pragma once

#include "Asset.h"

#include "Storage.h"

class ElementAsset
	: public Asset
{
	friend class ElementEditor;

	double	m_mass;
	double	m_radius;
	Color	m_color;
	Storage	m_material;

public:
	void	makeViewer() override;

	// Get
	double	getMass() const { return m_mass; }
	double	getRadius() const { return m_radius; }
	const Color& getColor() const { return m_color; }
	const Storage& getMaterial() const { return m_material; }

	// JSON
	void	load(const ptree& pt) override;
	void	save(ptree& pt) const override;
};