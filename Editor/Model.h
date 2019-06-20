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

	ptree	AddToJSON(ptree pt) const;
	void	SetFromJSON(const ptree& pt);

	virtual string	GetFilename() const;
	virtual string	GetFilepath() const;

	virtual void	Load(const ptree& pt) { SetFromJSON(pt); }
	virtual ptree	Save() const { return AddToJSON(ptree()); }
};
