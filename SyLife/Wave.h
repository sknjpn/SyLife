#pragma once

#include "Model.h"
#include "ModelViewer.h"

class WaveViewer;

class WaveModel
	: public Model
{
	friend class WaveViewer;

	double	m_force;	// ”g‚Ì‹­‚³
	double	m_interval;	// ŠiŽq“_ŠÔŠu

public:
	// get
	double	GetForce() const { return m_force; }
	double	GetInterval() const { return m_interval; }

	void	MakeViewer() override;
};

class WaveViewer
	: public ModelViewer
{
public:
	// reload
	void	ReloadProperties_this() { ModelViewer::ReloadProperties_this(); }
	void	ReloadProperties() override { ReloadProperties_this(); }

	// update
	void	Update_this() { ModelViewer::Update_this(); }
	void	Update() override { Update_this(); }
};

inline void WaveModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<WaveViewer>()->SetModel(shared_from_this());
}
