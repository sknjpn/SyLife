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
	{}

	void	Update() override;
};

