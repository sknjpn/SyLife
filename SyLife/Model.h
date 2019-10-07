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
	void	load_this(const ptree& pt);
	virtual void	load(const ptree& pt) { load_this(pt); }
	void	save_this(ptree& pt) const;
	virtual void	save(ptree& pt) const { save_this(pt); }
};
