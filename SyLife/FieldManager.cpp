#include "FieldManager.h"
#include "ChipManager.h"

unique_ptr<FieldManager>	g_fieldManagerPtr;

void FieldManager::init()
{
	m_size = g_chipManagerPtr->getRect().size* g_chipManagerPtr->getLength();
}
