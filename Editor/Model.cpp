#include "Model.h"

#include "ViewerManager.h"
#include "Viewer.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>

void Model::SetName(const string& name)
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

void Model::MakeViewers()
{
	g_viewerManagerPtr->MakeViewer<Viewer>();
}

void Model::Save_this(ptree& pt) const
{
	// name
	pt.put("name", m_name);

	// type
	pt.put("type", "Model");
}

void Model::Load_this(const ptree& pt)
{
	m_name = pt.get<string>("name");
}

string Model::GetFilename() const
{
	auto filename = m_name + ".json";
	auto f = [](unsigned char c) { return char(c == 0x20 ? 0x5f : tolower(c)); };

	transform(filename.begin(), filename.end(), filename.begin(), f);

	return filename;
}

string Model::GetFilepath() const
{
	return "assets/model/" + GetFilename();
}

