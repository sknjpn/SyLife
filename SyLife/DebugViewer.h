#pragma once

#include "Viewer.h"

class DebugViewer :
	public Viewer
{
	enum struct Mode {
		ShowViewerManager,
		ShowCellManager,
		ShowElementManager,
		ShowEggManager,
		ShowChipManager,
	} m_mode;

public:
	void	init() override;

	void	update() override;
};

