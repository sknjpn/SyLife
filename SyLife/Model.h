#pragma once

#include "Viewer.h"

#include <boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

#include <boost/filesystem/operations.hpp>
using namespace boost::filesystem;

class Model
	: public enable_shared_from_this<Model>
{
public:
	virtual ~Model() = default;

	virtual void	makeViewer() {}

	// JSON
	virtual void	load(const ptree& pt) {}
	virtual void	save(ptree& pt) const {}
};
