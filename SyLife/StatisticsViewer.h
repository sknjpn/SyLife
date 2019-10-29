#pragma once

#include "Viewer.h"

class CellAsset;

class StatisticsViewer :
	public Viewer
{
	struct Log
	{
		struct Status
		{
			int		m_num;

			Status(const shared_ptr<CellAsset>& cellAsset);
		};

		shared_ptr<CellAsset>	m_cellAsset;
		Array<Status>	m_statuses;

		Log(const shared_ptr<CellAsset>& cellAsset)
			: m_cellAsset(cellAsset)
		{}
	};

	Array<Log>	m_logs;
	int	m_statusesSizeMax = 1000;

public:
	void	update() override;
	void	init() override;
};

