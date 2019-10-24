#include "StatisticsViewer.h"

#include "CellAsset.h"
#include "CellMakingViewer.h"

#include "AssetManager.h"

void StatisticsViewer::init()
{
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
}

StatisticsViewer::Log::Status::Status()
{
}
