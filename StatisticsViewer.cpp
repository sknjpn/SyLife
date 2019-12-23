#include "StatisticsViewer.h"
#include "CellState.h"
#include "CellAsset.h"
#include "EggState.h"
#include "World.h"
#include "Assets.h"

void StatisticsViewer::update()
{
	DraggableViewer::update();

	RectF(getViewerSize()).rounded(16).draw(Palette::Gray).drawFrame(2.0, 0.0, Palette::Black);

	RectF(getViewerSize()).stretched(-20).draw(Palette::Black);

	moveDrawPos(20, 20);

	// Logの表示
	{
		double max = 100;
		for (auto& l : m_logs)
			for (auto& s : l.m_statuses) max = Max(max, double(s.m_num));

		for (int i = 0; i < m_logs.size(); ++i)
		{
			auto& l = m_logs[i];

			moveDrawPos(double(m_statusesSizeMax - l.m_statuses.size()), 0.0);
			
			for (int j = 0; j < l.m_statuses.size() - 1; ++j)
			{
				Vec2 p0(j, (1.0 - (l.m_statuses[j].m_num / max)) * 100.0);
				Vec2 p1(j + 1, (1.0 - (l.m_statuses[j + 1].m_num / max)) * 100.0);
				Line(p0, p1).draw(HSV(i * 60));
			}

			moveDrawPos(double(l.m_statuses.size() - m_statusesSizeMax), 0.0);
		}
	}

	setDrawPos(0, 0);
	if (isMouseover())
		RectF(getViewerSize()).rounded(16).draw(ColorF(1.0, 0.25));
}

void StatisticsViewer::init()
{
	setViewerRectInLocal(50, 1080 - 150, m_statusesSizeMax + 40, 140);
}

void StatisticsViewer::takeLog()
{
	static int count = 0;
	if (++count == 16) count = 0;
	else return;

	// Logの追加
	auto cas = Assets::GetAssets<CellAsset>();
	for (auto& ca : cas)
		if (m_logs.all([&ca](const auto& l) { return l.m_cellAsset != ca; }))
			m_logs.emplace_back(ca);

	// Logの更新
	for (auto& l : m_logs)
	{
		l.m_statuses.emplace_back(l.m_cellAsset);
		while (l.m_statuses.size() > m_statusesSizeMax) l.m_statuses.pop_front();
	}
}

StatisticsViewer::Log::Status::Status(const shared_ptr<CellAsset>& cellAsset)
{
	m_num = int(World::GetInstance()->getField().getCellStates().count_if([&cellAsset](const auto& cs) { return cs->getCellAsset() == cellAsset; }));
	m_num = int(World::GetInstance()->getField().getEggStates().count_if([&cellAsset](const auto& es) { return es->getCellAsset() == cellAsset; }));
}
