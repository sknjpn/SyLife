#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

class Model
{
	string	m_name;

public:
	virtual ~Model() = default;

	// Get
	const string&	GetName() const { return m_name; }

	// JSON
	void	SetFromJSON(const ptree& pt) { m_name = pt.get<string>("name"); }
	virtual void	Load(const ptree& pt) { SetFromJSON(pt); }
};
