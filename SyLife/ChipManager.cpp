#include "ChipManager.h"

unique_ptr<ChipManager> g_chipManagerPtr;

ChipManager::ChipManager()
	: m_size(32, 32)
	, m_length(100)
{

}

Point ChipManager::getPoint(const Vec2& position) const
{
	return Point(int(position.x / m_length), int(position.y / m_length));
}
