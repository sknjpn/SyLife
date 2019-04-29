#pragma once

#include "Rigidbody.h"

class Rigidbody;

class FieldManager
{
public:
	vector<shared_ptr<Rigidbody>>	m_rigidbodies;

	RigidbodyCloud m_cloud;
	RigidbodyAdaptor m_index;

public:
	FieldManager();
	~FieldManager();

	void	Init();
	void	Update();

	double	GetDeltaTime() const { return 1.0 / 60.0; }

	std::vector<std::pair<size_t, int>>	GetNearRigidbodies(Vector2D position, double radius) const;
};

extern unique_ptr<FieldManager>	g_fieldManager;