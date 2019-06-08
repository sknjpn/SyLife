#include "AssetManager.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

#include "Body.h"
#include "Equipment.h"
#include "Module.h"

using namespace boost::property_tree;

void	AssetManager::LoadPartModel(const string& filepath)
{
	ptree pt;
	read_json(filepath, pt);

	if (auto type_str = pt.get_optional<string>("type"))
	{
		if (type_str.get() == "Body")
		{
			const auto& model = g_assetManagerPtr->m_partModels.emplace_back(make_shared<BodyModel>());

			// Name
			model->m_name = pt.get<string>("name");

			// Mass
			model->m_mass = pt.get<double>("mass");

			s3d::Logger << s3d::Unicode::Widen(model->m_name);
		}

		if (type_str.get() == "Equipment")
		{
			const auto& model = g_assetManagerPtr->m_partModels.emplace_back(make_shared<EquipmentModel>());

			// Name
			model->m_name = pt.get<string>("name");

			// Mass
			model->m_mass = pt.get<double>("mass");

			s3d::Logger << s3d::Unicode::Widen(model->m_name);
		}

		if (type_str.get() == "Module")
		{
			const auto& model = g_assetManagerPtr->m_partModels.emplace_back(make_shared<ModuleModel>());

			// Name
			model->m_name = pt.get<string>("name");

			// Mass
			model->m_mass = pt.get<double>("mass");

			s3d::Logger << s3d::Unicode::Widen(model->m_name);
		}
	}
}