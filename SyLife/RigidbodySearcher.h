#pragma once

#include "RigidbodyCloud.h"

class RigidbodySearcher
{
public:
	RigidbodyCloud m_cloud;
	RigidbodyAdaptor m_index;

public:
	RigidbodySearcher();
	~RigidbodySearcher();

	std::vector<std::pair<size_t, int>>	GetNearRigidbodies(Vector2D position, double radius) const;
};

extern unique_ptr<RigidbodySearcher> g_rigidbodySearcherPtr;