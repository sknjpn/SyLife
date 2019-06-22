#include "CellModel.h"

#include "BodyConfig.h"
#include "EquipmentConfig.h"
#include "ModuleConfig.h"

#include "ChloroplastConfig.h"
#include "EyeConfig.h"
#include "WingConfig.h"

void CellModel::AddPartConfig(const ptree& pt)
{
	auto type = pt.get<string>("type");

	if (type == "BodyConfig")			AddPartConfig<BodyConfig>(pt);
	if (type == "EquipmentConfig")		AddPartConfig<EquipmentConfig>(pt);
	if (type == "ModuleConfig")			AddPartConfig<ModuleConfig>(pt);

	if (type == "ChloroplastConfig")	AddPartConfig<ChloroplastConfig>(pt);
	if (type == "EyeConfig")			AddPartConfig<EyeConfig>(pt);
	if (type == "WingConfig")			AddPartConfig<WingConfig>(pt);

}