#pragma once

#include "Asset.h"

#include "Storage.h"
#include "Shape.h"

class PartState;

class PartAsset
	: public Asset
{
public:
	friend class PartEditor;

	double	m_mass;
	Storage	m_material;
	Shape	m_shape;

public:
	// Get
	double	getMass() const { return m_mass; }
	const Storage& getMaterial() const { return m_material; }
	const Shape& getShape() const { return m_shape; }

	void	makeViewer() override;
	virtual shared_ptr<PartState>	makeState();
	void	draw(double a = 0.5) { m_shape.draw(a); }

	// JSON
	void	load(const JSONValue& json) override;
	void	save(ptree& pt) const override;
};
