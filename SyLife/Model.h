#pragma once

#include "ViewerManager.h"
#include "Viewer.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

class Model
	: public enable_shared_from_this<Model>
{
	string	m_name;

public:
	virtual ~Model() = default;

	virtual shared_ptr<Viewer>	MakeViewer() { return g_viewerManagerPtr->MakeViewer<Viewer>(); }

	// Set
	void	SetName(const string& name)
	{
		// ファイルの削除
		{
			const boost::filesystem::path path(GetFilepath());

			boost::filesystem::remove(path);
		}

		// nameのセット
		m_name = name;

		// 新規ファイルの作成
		{
			ptree pt;

			Save(pt);

			write_json(GetFilepath(), pt);
		}
	}

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
