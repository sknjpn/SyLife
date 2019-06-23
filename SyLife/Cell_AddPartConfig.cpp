#include "Cell.h"

#include "Body.h"
#include "Equipment.h"
#include "Module.h"

#include "Chloroplast.h"
#include "Eye.h"
#include "Wing.h"

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