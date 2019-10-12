#pragma once

#include "Viewer.h"

#include "SlideBar.h"
#include "ColorPicker.h"

class PartAsset;

class ShapeLayerViewer :
	public Viewer
{
	int		m_selectedIndex = 0;
	double	m_itemHeight = 80;
	SlideBar	m_slideBar;
	ColorPicker	m_colorPicker;
	shared_ptr<PartAsset>	m_partAsset;

public:
	ShapeLayerViewer();

	void	init() override;
	void	update() override;

	void	setPartAsset(const shared_ptr<PartAsset>& partAsset) { m_partAsset = partAsset; }

	void	drawLayers();

	int		getSelectedIndex() const { return m_selectedIndex; }
};

