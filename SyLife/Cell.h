#pragma once

#include "Rigidbody.h"
#include "Storage.h"
#include "Nucleus.h"

class Part;

class Cell : public Rigidbody
{
public:
	Nucleus m_nucleus;
	Storage	m_storage;
	vector<shared_ptr<Part>>	m_part;

public:
	Cell();
	~Cell();

	void	Update();
	void	Draw();
};

