#pragma once

#include "Model.h"
#include "ShapeModel.h"

class PartModel
	: public Model
{
public:
	double		m_mass;
	vector<ShapeModel>	m_shapes;

public:
	s3d::RectF	GetApproximateRect() const;
	double		GetRectInertia() const;
	s3d::Vec2	GetCenter() const;

	ptree	AddToJSON(ptree pt) const;
	void	SetFromJSON(const ptree& pt);

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	Save() const override { return AddToJSON(ptree()); }
};
