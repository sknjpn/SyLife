#pragma once

#include "Viewer.h"

class ShapeModel;

class ShapeLayerViewer :
	public Viewer
{
	shared_ptr<ShapeModel>	m_shapeModel;

public:
	void	init() override;
	void	update() override;

	void	setShapeModel(const shared_ptr<ShapeModel>& shapeModel);
};

