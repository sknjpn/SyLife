#pragma once

#include "Model.h"

class ShapeModel
	: public Model
{
public:
	Color	m_color;
	Array<Vec2>	m_verticles;

public:
	ShapeModel()
		: m_color(RandomHSV())
	{}
	ShapeModel(const Array<Vec2>& verticles)
		: m_verticles(verticles)
		, m_color(RandomHSV())
	{}

	void	Load_this(const ptree& pt);
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const;
	void	Save(ptree& pt) const override { Save_this(pt); }
};

inline void ShapeModel::Load_this(const ptree& pt)
{
	// color
	m_color = Color(pt.get<int>("color.r"), pt.get<int>("color.g"), pt.get<int>("color.b"));

	// verticles
	for (auto v : pt.get_child("verticles"))
		m_verticles.emplace_back(v.second.get<double>("x"), v.second.get<double>("y"));

	Model::Load_this(pt);
}

inline void ShapeModel::Save_this(ptree& pt) const
{
	// color
	{
		ptree color;

		color.put("r", m_color.r);
		color.put("g", m_color.g);
		color.put("b", m_color.b);

		pt.add_child("color", color);
	}

	// verticles
	{
		ptree verticles;

		{
			ptree verticle;

			for (const auto& v : m_verticles)
			{
				verticle.put("x", v.x);
				verticle.put("y", v.y);

				verticles.push_back(std::make_pair("", verticle));
			}
		}

		pt.add_child("verticles", verticles);
	}

	Model::Save_this(pt);

	// type
	pt.put("type", "ShapeModel");
}
