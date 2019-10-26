#pragma once

#include "Viewer.h"
#include "TinyCamera.h"

class CellAsset;
class BodyAsset;
class PartConfig;

class AssemblyViewer
	: public Viewer
{
	const shared_ptr<CellAsset>& getCellAsset() const;
	const shared_ptr<BodyAsset>& getBodyAsset() const;

public:
	TinyCamera	m_camera;

	double	m_mass;
	double	m_inertia;
	double	m_radius;
	Vec2	m_size;
	shared_ptr<PartConfig>	m_selectedPartConfig;

public:
	void	init() override;

	void	update() override;

	void	setSize(const Vec2& size);
	
	void	drawParts() const;
	void	drawGrid() const;
};

