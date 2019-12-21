#pragma once

#include "Viewer.h"
#include "TinyCamera.h"

class GridViewer
	: public Viewer
{
	TinyCamera	m_camera;

public:
	void	init() override;
	void	update() override;

	void	setSize(const Vec2& size);

	void	drawGrid() const;

	const TinyCamera& getCamera() const { return m_camera; }
};

