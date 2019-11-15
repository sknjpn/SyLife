#pragma once

#include "Viewer.h"

class CellAsset;

class CellBookViewer
	: public Viewer
{
public:
	class CellList
		: public Viewer
	{
	public:
		void	init() override;
		void	update() override;
	};

	class CellInfo
		: public Viewer
	{
		shared_ptr<CellAsset>	m_cellAsset;

	public:
		CellInfo(const shared_ptr<CellAsset>& cellAsset)
			: m_cellAsset(cellAsset)
		{}

		void	init() override;
		void	update() override;
	};

public:
	void	init() override;
	void	update() override;
};

