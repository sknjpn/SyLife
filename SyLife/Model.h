#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

class Model
{
public:
	string	m_name;

public:
	virtual ~Model() = default;

	void	SetFromJSON(const ptree& pt);
	virtual void	Load(const ptree& pt) { SetFromJSON(pt); }

	virtual string	GetFilename() const;
	virtual string	GetFilepath() const;
};
