#include "Model.h"
#include <boost/property_tree/json_parser.hpp>

void Model::Load(const string& filepath)
{
	ptree pt;
	read_json(filepath, pt);

	FromJSON(pt);
}

void Model::Save() const
{
	write_json(GetFilepath(), ToJSON());
}

string Model::GetFilename() const
{
	auto filename = m_name + ".json";
	auto f = [](unsigned char c) { return c == 0x20 ? 0x5f : tolower(c); };

	transform(filename.begin(), filename.end(), filename.begin(), f);

	return filename;
}

string Model::GetFilepath() const
{
	return "assets/model/" + GetFilename();
}

