#pragma once

#include "Viewer.h"

class CellAsset;

class ReleaseViewer
	: public Viewer
{
	bool	m_isDragged;

public:
	ReleaseViewer();

	bool	isDragged() const { return m_isDragged; }

	void	update() override;
};

