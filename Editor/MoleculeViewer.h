#pragma once

#include "Viewer.h"
#include "Molecule.h"

class MoleculeViewer
	: public Viewer
{
public:
	shared_ptr<MoleculeModel>	m_model;

public:
	MoleculeViewer(const shared_ptr<MoleculeModel>& model)
		: m_model(model)
	{
		m_drawRect = s3d::RectF(0, 0, 600, 600);
	}

	void	Update() override;
};

