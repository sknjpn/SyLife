#pragma once

#include "Indexer.h"

class Rigidbody;

class FieldManager
{
	Indexer<Rigidbody>	m_indexer;
	Vector2D	m_size;

public:
	FieldManager()
		: m_size(1920, 1080)
	{}

	void	Init();
	void	Update();
	void	LoadGenerationSetting();

	const Vector2D&	GetSize() const { return m_size; }

	Indexer<Rigidbody>&	GetIndexer() { return m_indexer; }
	const Indexer<Rigidbody>&	GetIndexer() const { return m_indexer; }

	double	GetDeltaTime() const { return 1.0 / 60.0; }
};

extern unique_ptr<FieldManager>	g_fieldManagerPtr;