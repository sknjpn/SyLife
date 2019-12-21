#pragma once

#include "Viewer.h"
#include "TinyCamera.h"

class PartAsset;

class PartShapeViewer
	: public Viewer
{
public:
	Array<Vec2>	m_verticles;

	Vec2* m_selectedVerticle = nullptr;

	TinyCamera	m_camera;

	shared_ptr<PartAsset>	m_partAsset;

public:
	PartShapeViewer(const shared_ptr<PartAsset>& partAsset)
		: m_partAsset(partAsset)
	{}

	void	init() override;
	void	update() override;
};

