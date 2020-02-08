#pragma once

#include "Asset.h"

#include "Storage.h"
#include "Shape.h"

class PartState;
class PartConfig;

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
	virtual std::shared_ptr<PartState>	makePartState(const std::shared_ptr<PartConfig>& partConfig) const = 0;

	// Draw
	virtual void	draw(double a = 0.5) { m_shape.draw(a); }

	virtual void	preRender(Image& image, double scale, const std::shared_ptr<PartConfig>& partConfig, double a = 0.5) const;

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	
	// TypeName
	String	getTypeName() override { return U"PartAsset"; }
};
