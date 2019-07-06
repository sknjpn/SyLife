#pragma once

#include "Indexer.h"

class Rigidbody;

class FieldManager
{
	Indexer<Rigidbody>	m_indexer;

public:
	FieldManager() {}

	// Get
	Indexer<Rigidbody>&	GetIndexer() { return m_indexer; }
	const Indexer<Rigidbody>&	GetIndexer() const { return m_indexer; }

	constexpr double	GetDeltaTime() const { return 1.0 / 60.0; }

	void	Init();
	void	Update();
};

extern unique_ptr<FieldManager>	g_fieldManagerPtr;
