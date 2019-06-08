#pragma once

class Shape
{
public:
	s3d::HSV	m_color;
	s3d::Array<s3d::Vec2>	m_verticles;

public:
	Shape(const s3d::Array<s3d::Vec2>& verticles)
		: m_verticles(verticles)
		, m_color(s3d::RandomHSV())
	{}
};

