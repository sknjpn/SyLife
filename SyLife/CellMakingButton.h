#pragma once

#include "Viewer.h"

class CellMakingButton :
	public Viewer
{
	bool	m_isClicked = false;

	void	onPushed();

public:
	CellMakingButton();

	void	update() override;
};

