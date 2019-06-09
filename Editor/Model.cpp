#include "Model.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

void Model::Load(const string& filepath)
{
	m_filepath = filepath;

	write_json(m_filepath, ptree());
}

void Model::Save() const
{
	write_json(m_filepath, ToJSON());
}

