#pragma once

#include "Model.h"
#include "ModelEditor.h"

class WaveViewer;

class WaveModel
	: public Model
{
	friend class WaveViewer;

	double	m_force;	// 波の強さ
	double	m_interval;	// 格子点間隔

public:
	// get
	double	GetForce() const { return m_force; }
	double	GetInterval() const { return m_interval; }

	void	makeViewer() override;
};

class WaveViewer
	: public ModelEditor
{
public:
	// reload
	void	reloadProperties_this() { ModelEditor::reloadProperties_this(); }
	void	reloadProperties() override { reloadProperties_this(); }

	// update
	void	Update_this() { ModelEditor::Update_this(); }
	void	Update() override { Update_this(); }
};
