#pragma once

#include "Part.h"

class PartManager
{
public:
	vector<shared_ptr<Part::Model>>	m_models;

public:
	PartManager();
	~PartManager();

	const shared_ptr<Part::Model>&	AddModel();
	const shared_ptr<Part::Model>&	GetModel(const string& name) const;
};

extern shared_ptr<PartManager>	g_partManagerPtr;