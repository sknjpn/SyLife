#pragma once

#include "Viewer.h"
#include "Cell.h"

class CellViewer :
	public Viewer
{
public:
	shared_ptr<CellModel>	m_model;

public:
	CellViewer(const shared_ptr<CellModel>& model)
		: m_model(model)
	{}

	void	Update() override;
};

