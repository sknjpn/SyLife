#include "ChipManager.h"
#include "Chip.h"
#include "FieldViewer.h"

unique_ptr<ChipManager> g_chipManagerPtr;

ChipManager::ChipManager()
	: m_rect(32, 32)
	, m_length(100)
{

}

Point ChipManager::getPoint(const Vec2& position) const
{
	return Point(int(position.x / m_length), int(position.y / m_length));
}

void ChipManager::init()
{
	// Chips
	{
		const Size size(128, 128);

		for (auto p : step(size))
			m_chips[p] = MakeShared<Chip>();

		// 結びつきの作成
		for (auto p : step(size.movedBy(-1, -1)))
		{
			m_chips[p]->m_r = m_chips[p.movedBy(1, 0)];
			m_chips[p]->m_d = m_chips[p.movedBy(0, 1)];
			m_chips[p.movedBy(1, 0)]->m_l = m_chips[p];
			m_chips[p.movedBy(0, 1)]->m_u = m_chips[p];
		}
	}

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

		auto v = g_waveManagerPtr->GetWaveVelocity(p * m_length);
		Line(p * m_length, p * m_length + v)
			.movedBy(m_length / 2.0, m_length / 2.0)
			.drawArrow(5.0, Vec2(25, 25));
	}
}

void ChipManager::updateChips()
{
}
