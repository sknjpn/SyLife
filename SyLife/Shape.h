﻿#pragma once

#include "Model.h"

#include "Layer.h"

class Shape
	: public Model
	, public Array<Layer>
{
public:
	void	load_this(const ptree& pt)
	{
		// layers
		for (auto m : pt.get_child("layers"))
		{
			emplace_back().load(m.second);
		}

		Model::load_this(pt);
	}
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const
	{
		// layers
		{
			ptree layers;

			for (const auto& m : *this)
			{
				ptree pt;
				m.save(pt);

				layers.push_back(std::make_pair("", pt));
			}

			pt.add_child("layers", layers);
		}


		Model::save_this(pt);

		// type
		pt.put("type", "Shape");
	}
	void	save(ptree& pt) const override { save_this(pt); }
};

