#pragma once

#include "Viewer.h"

#include "AssetManager.h"

#include "ShapeModel.h"

#include "SlideBar.h"

class PartAsset;

class PartPaletteViewer
	: public Viewer
{
	double	m_itemWidth = 80;
	SlideBar	m_slideBar;
	shared_ptr<PartAsset>	m_selectedPart;

public:
	PartPaletteViewer();

	const shared_ptr<PartAsset>& getSelectedPart() const { return m_selectedPart; }

	void	clearSelectedPart() { m_selectedPart = nullptr; }

	void	drawModels();

	void	update() override;
};

