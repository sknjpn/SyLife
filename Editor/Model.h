#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

class Viewer;

class Model
	: public enable_shared_from_this<Model>
{
public:
	string	m_name;

public:
	virtual ~Model() = default;

	virtual void	MakeViewers();

	virtual string	GetFilename() const;
	virtual string	GetFilepath() const;

	void	SetFromJSON(const ptree& pt);
	virtual void	Load(const ptree& pt) { SetFromJSON(pt); }
	ptree	AddToJSON(ptree pt) const;
	virtual ptree	Save() const { return AddToJSON(ptree()); }
};
