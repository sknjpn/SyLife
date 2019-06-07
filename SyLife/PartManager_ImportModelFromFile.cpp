#include "PartManager.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

#include "Body.h"
#include "Equipment.h"
#include "Module.h"

using namespace boost::property_tree;

void PartManager::ImportModelFromFile(const string& filePath)
{
	ptree pt;
	read_json(filePath, pt);

	if (boost::optional<std::string> type_str = pt.get_optional<std::string>("type"))
	{
		if (type_str.get() == "Body")
		{
			const auto& model = g_partManagerPtr->AddModel<BodyModel>();

			// Data.value
			if (boost::optional<double> value = pt.get_optional<double>("mass")) model->m_mass = value.get();
			else model->m_mass = 10.0;

			// Data.str
			if (boost::optional<std::string> str = pt.get_optional<std::string>("name")) model->m_name = str.get();
			else model->m_name = "str is nothing";

			s3d::Logger << s3d::Unicode::Widen(model->m_name);
		}

		if (type_str.get() == "Equipment")
		{
			const auto& model = g_partManagerPtr->AddModel<EquipmentModel>();

			s3d::Logger << s3d::Unicode::Widen(model->m_name);
		}

		if (type_str.get() == "Module")
		{
			const auto& model = g_partManagerPtr->AddModel<ModuleModel>();

			s3d::Logger << s3d::Unicode::Widen(model->m_name);
		}
	}
}
