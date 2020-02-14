#pragma once

#include "Asset.h"

#include "Storage.h"

class ProteinAsset
	: public Asset
{
	double	m_radius;
	Color	m_color;
	Storage	m_material;

public:
	// Get
	double	getRadius() const { return m_radius; }
	const Color& getColor() const { return m_color; }
	const Storage& getMaterial() const { return m_material; }

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	
	// TypeName
	String	getTypeName() override { return U"ProteinAsset"; }

	void	makeEditor(const std::shared_ptr<EasyViewer>& parent) const override;
};