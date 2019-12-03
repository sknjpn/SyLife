﻿#pragma once

#include "Viewer.h"

class WorldGenerateViewer :
	public Viewer
{
	String	m_name;

	void	generateWorld();

public:
	void	init() override;
	void	update() override;
};
