#pragma once

#include <boost/property_tree/ptree.hpp>

using namespace boost::property_tree;

class Model
{
public:
	string	m_name;

public:
	virtual ~Model() = default;

	virtual ptree	ToJSON() const = 0;
	virtual void	Load(const string& filepath);
	virtual void	Save() const;

	virtual string	GetFilename() const;
	virtual string	GetFilepath() const;
};
