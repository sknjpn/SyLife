﻿#include "StatisticsViewer.h"

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
	for(int i = 0; i < m_logs.size(); ++i)
	{
		auto& l = m_logs[i];
		double max = 100;
		for (auto& s : l.m_statuses) max = Max(max, double(s.m_num));
		for (int j = 0; j < l.m_statuses.size() - 1; ++j)
		{
			Vec2 p0(j, (1.0 - (l.m_statuses[j].m_num / max)) * 100.0);
			Vec2 p1(j + 1, (1.0 - (l.m_statuses[j + 1].m_num / max)) * 100.0);
			Line(p0, p1).draw(HSV(i * 60));
		}
	}
}

StatisticsViewer::Log::Status::Status()
{
	m_num = g_cellManagerPtr->getCellStates().size();
}
