#include "CellModel.h"

ptree CellModel::AddToJSON(ptree pt) const
{
	// parts
	{
		ptree parts;

		for (const auto& e : m_parts)
			parts.push_back(std::make_pair("", e->Save()));

		pt.add_child("parts", parts);
	}

	return Model::AddToJSON(pt);
}

void CellModel::SetFromJSON(const ptree & pt)
{
	// parts
	for (auto part : pt.get_child("parts"))
	{
		shared_ptr<PartConfig> pc;

		if (part.second.get<string>("type") == "Body") m_parts.emplace_back(m_body = make_shared<BodyConfig>())->Load(part.second);
		if (part.second.get<string>("type") == "Equipment") m_parts.emplace_back(m_equipments.emplace_back(make_shared<EquipmentConfig>()))->Load(part.second);
		if (part.second.get<string>("type") == "Module") m_parts.emplace_back(m_modules.emplace_back(make_shared<ModuleConfig>()))->Load(part.second);
	}

	Model::SetFromJSON(pt);
}
