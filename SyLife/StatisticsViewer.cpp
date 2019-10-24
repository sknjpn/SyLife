#include "StatisticsViewer.h"

#include "CellAsset.h"
#include "CellManager.h"

#include "AssetManager.h"

void StatisticsViewer::init()
{
	setDrawSize(m_statusesSizeMax, 100);
}

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
		l.m_statuses.emplace_back();
		while (l.m_statuses.size() > m_statusesSizeMax) l.m_statuses.pop_front();
	}

	// Logの表示
	for (auto& l : m_logs)
	{
		int max = 1;
		for (auto& s : l.m_statuses) max = Max(max, s.m_num);
		for (int i = 0; i < l.m_statuses.size() - 1; ++i)
		{
			Vec2 p0(i, l.m_statuses[i].m_num * 100.0 / max);
			Vec2 p1(i + 1, l.m_statuses[i + 1].m_num * 100.0 / max);
			Line(p0, p1).draw();
		}
	}
}

StatisticsViewer::Log::Status::Status()
{
	m_num = g_cellManagerPtr->getCellStates().size();
}
