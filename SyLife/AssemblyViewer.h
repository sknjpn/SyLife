#pragma once

#include "Viewer.h"
#include "TinyCamera.h"

class CellAsset;
class PartConfig;

class AssemblyViewer
	: public Viewer
{
	shared_ptr<CellAsset> m_cellAsset;

	enum struct State
	{
		MoveMode,
		RotateMode,
	} m_state = State::MoveMode;

	TinyCamera	m_camera;

	double	m_mass;
	double	m_inertia;
	double	m_radius;
	Vec2	m_size;
	bool	m_grabCircle;
	shared_ptr<PartConfig>	m_selectedPartConfig;

public:
	AssemblyViewer();

	void	update() override;

	void	setSize(const Vec2& size);

	void	drawParts() const;
	void	drawGrid() const;

	void	setCellAsset(const shared_ptr<CellAsset>& cellAsset) { m_cellAsset = cellAsset; }
};

