#pragma once

#include "AssetModel.h"

#include "Storage.h"
#include "ShapeModel.h"

class PartState;

class PartModel
	: public AssetModel
{
public:
	friend class PartEditor;

	double	m_mass;
	Storage	m_material;
	Array<ShapeModel>	m_shapes;

public:
	// Get
	double	getMass() const { return m_mass; }
	const Storage& getMaterial() const { return m_material; }
	const Array<ShapeModel>& getShapes() const { return m_shapes; }

	void	makeViewer() override;
	virtual shared_ptr<PartState>	makeState();

	void	Draw(double a = 0.5) { for (const auto& s : m_shapes) s.Draw(a); }

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
	string	getFilepath() const override { return "assets/models/parts/" + getFilename(); }
};
