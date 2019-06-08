#include "AssetManager.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

using namespace boost::property_tree;

void	AssetManager::LoadMoleculeModel(const string& filepath)
{
	const auto& model = g_assetManagerPtr->m_moleculeModels.emplace_back(make_shared<MoleculeModel>());

	ptree pt;
	read_json(filepath, pt);

	// color
	{
		auto child = pt.get_child("color");
		int n = 0;

		for (auto it = child.begin(); n != 3; ++it)
		{
			model->m_color[n] = (*it).second.get_value<int>();

			++n;
		}
	}

	// mass
	model->m_mass = pt.get<double>("mass");

	// name
	model->m_name = pt.get<string>("name");

	// radius
	model->m_radius = sqrt(model->m_mass);

	s3d::Logger << s3d::Unicode::Widen(model->m_name);
}