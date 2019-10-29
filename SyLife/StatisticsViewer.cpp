#include "StatisticsViewer.h"

#include "CellState.h"
#include "CellAsset.h"
#include "CellManager.h"

#include "EggManager.h"
#include "EggState.h"
#include "AssetManager.h"

void StatisticsViewer::update()
{
	// Logの追加
	auto cas = g_assetManagerPtr->getAssets<CellAsset>();
	for (auto& ca : cas)
		if (m_logs.all([&ca](const auto& l) { return l.m_cellAsset != ca; }))
			m_logs.emplace_back(ca);

	// Logの更新
	for (auto& l : m_logs)
	{
		l.m_statuses.emplace_back(l.m_cellAsset);
		while (l.m_statuses.size() > m_statusesSizeMax) l.m_statuses.pop_front();
	}

	// Logの表示
	{
		double max = 100;
		for (auto& l : m_logs)
			for (auto& s : l.m_statuses) max = Max(max, double(s.m_num));

		for (int i = 0; i < m_logs.size(); ++i)
		{
			auto& l = m_logs[i];
			for (int j = 0; j < l.m_statuses.size() - 1; ++j)
			{
				Vec2 p0(j, (1.0 - (l.m_statuses[j].m_num / max)) * 100.0);
				Vec2 p1(j + 1, (1.0 - (l.m_statuses[j + 1].m_num / max)) * 100.0);
				Line(p0, p1).draw(HSV(i * 60));
			}
		}
	}
}

void StatisticsViewer::init()
{
	setViewerRect(50, 1080 - 150, m_statusesSizeMax, 100);
}

StatisticsViewer::Log::Status::Status(const shared_ptr<CellAsset>& cellAsset)
{
	m_num = int(g_cellManagerPtr->getCellStates().count_if([&cellAsset](const auto& cs) { return cs->getCellAsset() == cellAsset; }));
	m_num = int(g_eggManagerPtr->getEggStates().count_if([&cellAsset](const auto& es) { return es->getCellAsset() == cellAsset; }));
}
