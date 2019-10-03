#include "ChipManager.h"
#include "FieldViewer.h"

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

void ChipManager::drawChips()
{
	// 領域の取得
	Rect rect;
	{
		auto cameraRect = g_viewerManagerPtr->GetViewer<FieldViewer>()->getCamera().getCameraRect();

		rect.pos = getPoint(cameraRect.pos);
		rect.size = getPoint(cameraRect.br()) - rect.pos;
	}

	for (const auto p : step(rect.pos, rect.size))
	{
		// const auto& c = m_chips[p];

		RectF(p * m_length, Vec2(m_length, m_length)).drawFrame(2.0, Palette::White);
	}
}

void ChipManager::updateChips()
{
}
