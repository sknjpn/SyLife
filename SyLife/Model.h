#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

class Model
{
	string	m_name;

public:
	virtual ~Model() = default;

	// Set
	void	SetName(const string& name) { m_name = name; }

	// Get
	const string&	GetName() const { return m_name; }

	// Path
	virtual string	GetFilename() const
	{
		auto filename = m_name + ".json";
		auto f = [](unsigned char c) { return char(c == 0x20 ? 0x5f : tolower(c)); };

		transform(filename.begin(), filename.end(), filename.begin(), f);

		return filename;
	}
	virtual string	GetFilepath() const
	{
		return "assets/model/" + GetFilename();
	}


	// JSON
	void	SetFromJSON(const ptree& pt)
	{
		m_name = pt.get<string>("name");
	}
	virtual void	Load(const ptree& pt) { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const {
		// name
		pt.put("name", m_name);

		// type
		pt.put("type", "Model");
	}
	virtual void	Save(ptree& pt) const { AddToJSON(pt); }
};
