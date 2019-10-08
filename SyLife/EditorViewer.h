#pragma once

#include "Viewer.h"

class Model;

class EditorViewer
	: public Viewer
{
	shared_ptr<Model>	m_selectedAsset;

public:
	EditorViewer();

	void	init();

	void	update();
};

