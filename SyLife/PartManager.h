#pragma once

#include "Part.h"

class PartManager
{
public:
	vector<shared_ptr<PartModel>>	m_models;

public:
	PartManager() { m_models.reserve(10000); }

	template <typename T>
	const shared_ptr<T>&	AddModel() { auto t = make_shared<T>(); m_models.emplace_back(t); return t; }
	const shared_ptr<PartModel>&	GetModel(const string& name) const;

	void	Init();

	void	ImportModelFromFile(const string& filePath);
};

extern shared_ptr<PartManager>	g_partManagerPtr;