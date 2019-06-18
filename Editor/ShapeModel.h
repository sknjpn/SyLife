#pragma once

#include "Model.h"

class ShapeModel
	: public Model
{
public:
	s3d::HSV	m_color;
	s3d::Array<s3d::Vec2>	m_verticles;

public:
	ShapeModel(const s3d::Array<s3d::Vec2>& verticles)
		: m_verticles(verticles)
		, m_color(s3d::RandomHSV())
	{}
	ShapeModel()
		: m_verticles()
		, m_color(s3d::RandomHSV())
	{}

	ptree	ToJSON() const override;
	void	FromJSON(const ptree& pt) override;
};

