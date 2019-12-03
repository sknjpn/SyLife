#pragma once

#include "Asset.h"

#include "Storage.h"
#include "Shape.h"

class PartState;

class PartAsset
	: public Asset
{
protected:
	double	m_mass;
	Storage	m_material;
	Shape	m_shape;

public:
	// Get
	double	getMass() const { return m_mass; }
	const Storage& getMaterial() const { return m_material; }
	const Shape& getShape() const { return m_shape; }
	Storage& getMaterial() { return m_material; }
	Shape& getShape() { return m_shape; }

	// Set
	void	setMass(double mass) { m_mass = mass; }

	// State
	virtual shared_ptr<PartState>	makeState() = 0;

	// Draw
	void	draw(double a = 0.5) { m_shape.draw(a); }

	// Icon
	void	drawIcon() const override;

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	
	// TypeName
	String	getTypeName() override { return U"PartAsset"; }
};
