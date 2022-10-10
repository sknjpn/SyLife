﻿#include "CellAsset.h"
#include "CellState.h"
#include "EggState.h"
#include "MainViewer.h"
#include "World.h"

void MainViewer::StatisticsViewer::update()
{
  DraggableViewer::update();

  RectF(getViewerSize()).rounded(5).draw(Palette::Gray).drawFrame(1.0, 0.0, Palette::Black);

  RectF(getViewerSize()).stretched(-5).draw(Palette::Black);

  moveDrawPos(10, 10);

  // Logの表示
  {
    double max = 100;
    for (auto& l : m_logs)
      for (auto& s : l.m_statuses) max = Max(max, Max(double(s.m_numCell), double(s.m_numEgg)));

    for (int i = 0; i < m_logs.size(); ++i)
    {
      auto& l = m_logs[i];

      moveDrawPos(double(m_statusesSizeMax - l.m_statuses.size()), 0.0);

      for (int j = 0; j < l.m_statuses.size() - 1; ++j)
      {
        Vec2 p0(j, (1.0 - (l.m_statuses[j].m_numCell / max)) * 100.0);
        Vec2 p1(j + 1, (1.0 - (l.m_statuses[j + 1].m_numCell / max)) * 100.0);
        Line(p0, p1).draw(HSV(i * 60));
      }

      for (int j = 0; j < l.m_statuses.size() - 1; ++j)
      {
        Vec2 p0(j, (1.0 - (l.m_statuses[j].m_numEgg / max)) * 100.0);
        Vec2 p1(j + 1, (1.0 - (l.m_statuses[j + 1].m_numEgg / max)) * 100.0);
        Line(p0, p1).draw(ColorF(HSV(i * 60), 0.5));
      }

      moveDrawPos(double(l.m_statuses.size() - m_statusesSizeMax), 0.0);
    }
  }
}

void MainViewer::StatisticsViewer::init()
{
  setViewerRectInLocal(50, 1080 - 150, m_statusesSizeMax + 20, 120);
}

void MainViewer::StatisticsViewer::takeLog()
{
  static int count = 0;
  if (++count == 16)
    count = 0;
  else
    return;

  // Logの追加
  auto cas = World::GetAssets<CellAsset>();
  for (auto& ca : cas)
    if (m_logs.all([&ca](const auto& l) { return l.m_cellAsset != ca; }))
      m_logs.emplace_back(ca).m_statuses.resize(m_statusesSizeMax);

  // Logの更新
  for (auto& l : m_logs)
  {
    l.m_statuses.emplace_back(l.m_cellAsset);
    while (l.m_statuses.size() > m_statusesSizeMax) l.m_statuses.pop_front();
  }
}

MainViewer::StatisticsViewer::Log::Status::Status(const std::shared_ptr<CellAsset>& cellAsset)
{
  m_numCell = int(World::GetInstance()->getCellStates().count_if([&cellAsset](const auto& cs) { return cs->getCellAsset() == cellAsset; }));
  m_numEgg = int(World::GetInstance()->getEggStates().count_if([&cellAsset](const auto& es) { return es->getCellAsset() == cellAsset; }));
}
