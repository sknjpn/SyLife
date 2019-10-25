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
	const Shape& getShapes() const { return m_shape; }

	void	makeViewer() override;
	virtual shared_ptr<PartState>	makeState();

	void	draw(double a = 0.5) { for (const auto& l : m_shape) l.draw(a); }

	RectF	getApproximateRect() const;
	Vec2	getApproximateRectTopLeft() const;
	Vec2	getApproximateRectBottomDown() const;
	double	getRectInertia() const;
	Vec2	getCenter() const { return (getApproximateRectTopLeft() + getApproximateRectBottomDown()) / 2.0; }

	// JSON
	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
};
