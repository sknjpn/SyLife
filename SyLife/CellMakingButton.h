#pragma once

#include "Viewer.h"

class CellMakingButton :
	public Viewer
{
	bool	m_isClicked = false;
	bool	m_isSelected = false;

	void	onPushed();

public:
	CellMakingButton();

	void	update() override;

	bool	isSelected() const { return m_isSelected; }
};

