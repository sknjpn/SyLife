#pragma once

#include "Indexer.h"
#include "Rigidbody.h"

class FieldManager
{
public:
	Indexer<Rigidbody>	m_indexer;
	Vector2D	m_size;

public:
	FieldManager() : m_size(1920, 1080) {}

	void	Init();
	void	Draw();
	void	Update();
	void	LoadGenerationSetting();

	double	GetDeltaTime() const { return 1.0 / 60.0; }

	// ìùåv
	int		GetNumMoleculeAll() const;
};

extern unique_ptr<FieldManager>	g_fieldManagerPtr;