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

			Status();
		};

		shared_ptr<CellAsset>	m_cellAsset;
		Array<Status>	m_statuses;

		Log(const shared_ptr<CellAsset>& cellAsset) 
			: m_cellAsset(cellAsset)
		{}
	};

	Array<Log>	m_logs;
	int	m_statusesSizeMax = 100;

public:
	void	init() override;
	void	update() override;
};

