#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

class Viewer;

class Model
	: public enable_shared_from_this<Model>
{
	string	m_name;

public:
	const string&	GetName() const { return m_name; }
	void	SetName(const string& name);

	virtual ~Model() = default;

	virtual void	MakeViewers();

	virtual string	GetFilename() const;
	virtual string	GetFilepath() const;

	void	SetFromJSON(const ptree& pt);
	virtual void	Load(const ptree& pt) { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const;
	virtual void	Save(ptree& pt) const { AddToJSON(pt); }
};
