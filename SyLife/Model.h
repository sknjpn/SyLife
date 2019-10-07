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

	virtual void	MakeViewer() {}

	// JSON Load
	void	load_this(const ptree& pt)
	{

	}
	virtual void	load(const ptree& pt) { load_this(pt); }

	// JSON Save
	void	save_this(ptree& pt) const {
		// type
		pt.put("type", "Model");
	}
	virtual void	save(ptree& pt) const { save_this(pt); }
};
