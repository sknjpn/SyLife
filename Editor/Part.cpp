#include "Part.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>


string PartModel::ToJson() const
{
	ptree pt;

	pt.put("Data.value", 3);
	pt.put("Data.str", "Hello");

	ptree child;
	{
		ptree info;
		info.put("id", 1);
		info.put("name", "Alice");
		child.push_back(std::make_pair("", info));
	}
	{
		ptree info;
		info.put("id", 2);
		info.put("name", "Millia");
		child.push_back(std::make_pair("", info));
	}
	pt.add_child("Data.info", child);

	write_json("data_out.json", pt);
}
