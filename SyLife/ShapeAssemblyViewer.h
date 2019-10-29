#pragma once

#include "Viewer.h"
#include "TinyCamera.h"

class PartAsset;

class ShapeAssemblyViewer
	: public Viewer
{
	double	m_circleRadius = 10.0;
	Vec2	m_size;
	TinyCamera	m_camera;
	shared_ptr<PartAsset>	m_partAsset;

public:
	ShapeAssemblyViewer()
	{
		setBackgroundColor(Palette::Gray);

		setViewerRect(RectF(800, 800).setCenter(getDrawCenter().movedBy(0, -50)));
		setSize(Vec2(800, 800));
	}

	void	update() override;

	void	setPartAsset(const shared_ptr<PartAsset>& partAsset) { m_partAsset = partAsset; }
	void	setSize(const Vec2& size);

	void	drawGrid() const;
};
