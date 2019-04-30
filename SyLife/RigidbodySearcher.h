#pragma once

#include "RigidbodyCloud.h"

class Rigidbody;

class RigidbodySearcher
{
public:
	RigidbodyCloud m_cloud;
	RigidbodyAdaptor m_index;

public:
	RigidbodySearcher();
	~RigidbodySearcher();

	void	ForEachNearRigidbodies(Vector2D position, double radius, function<void(const shared_ptr<Rigidbody>&, double)> func) const;

	std::vector<std::pair<size_t, double>>	GetNearRigidbodies(Vector2D position, double radius) const;
};

extern unique_ptr<RigidbodySearcher> g_rigidbodySearcherPtr;