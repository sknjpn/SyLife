#include "stdafx.h"
#include "Shape.h"

void Shape::load_this(const ptree& pt)
{
	// layers
	for (auto m : pt.get_child("layers"))
	{
		emplace_back().load(m.second);
	}
}

void Shape::save_this(ptree& pt) const
{
	// layers
	{
		ptree layers;

		for (const auto& m : *this)
		{
			ptree pt2;
			m.save(pt2);

			layers.push_back(std::make_pair("", pt2));
		}

		pt.add_child("layers", layers);
	}
}
