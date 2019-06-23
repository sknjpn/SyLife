#pragma once

#include "Model.h"

class ShapeModel
	: public Model
{
public:
	s3d::Color	m_color;
	s3d::Array<s3d::Vec2>	m_verticles;

public:
	ShapeModel()
		: m_color(s3d::RandomHSV())
	{}
	ShapeModel(const s3d::Array<s3d::Vec2>& verticles)
		: m_verticles(verticles)
		, m_color(s3d::RandomHSV())
	{}

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const;
	void	Save(ptree& pt) const override { AddToJSON(pt); }
};

