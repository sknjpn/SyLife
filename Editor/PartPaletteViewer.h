#pragma once

#include "Viewer.h"

#include "CellModel.h"
#include "PartModel.h"

class PartPaletteViewer
	: public Viewer
{
public:
	shared_ptr<CellModel>	m_model;

	static shared_ptr<PartModel>	m_selectedPart;

public:
	PartPaletteViewer(const shared_ptr<CellModel>& model)
		: m_model(model)
	{
		m_drawRect = s3d::RectF(450, 0, 150, 450);
	}

	void	Update() override;


	static const shared_ptr<PartModel>&	GetSelectedPart() { return m_selectedPart; }
};

