#pragma once

#include <boost/property_tree/ptree.hpp>

using namespace boost::property_tree;

class Model
{
public:
	virtual ~Model() = default;

	virtual ptree	ToJSON() const = 0;
};

