#include "Cell.h"
#include "FieldManager.h"
#include "Chip.h"

unique_ptr<FieldManager>	g_fieldManagerPtr;

void FieldManager::Init()
{
	ptree pt;
	read_json("assets/generation.json", pt);
}
