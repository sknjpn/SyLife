#pragma once

#include "Cloud.h"

template<typename T>
class Indexer
{
public:
	Cloud<T>	m_cloud;
	Adaptor<T>	m_index;

public:
	Indexer() : m_index(2, m_cloud, KDTreeSingleIndexAdaptorParams(10000)) { }
	~Indexer() = default;

	void	ForEachNearParticles(Vector2D position, double radius, function<void(const shared_ptr<T>&, double)> func) const
	{
		for (const auto& l : GetNearParticles(position, radius)) func(m_cloud.m_particles[l.first], l.second);
	}

	std::vector<std::pair<size_t, double>>	GetNearParticles(Vector2D position, double radius) const
	{
		const double query_pt[2] = { position.m_x, position.m_y };
		const double search_radius = radius * radius;
		std::vector<std::pair<size_t, double>>   ret_matches;
		nanoflann::SearchParams params;
		params.sorted = false;	//ÇÊÇËëÅÇ≠
		const size_t nMatches = m_index.radiusSearch(&query_pt[0], search_radius, ret_matches, params);

		return ret_matches;
	}

	const vector<shared_ptr<T>>&	GetParticles() const { return m_cloud.m_particles; }
	
	void	Update() const 
	{
		m_cloud.m_particles.erase(remove_if(m_cloud.m_particles.begin(), m_cloud.m_particles.end(), [](const auto& r) { return r->m_destroyFlag; }), m_cloud.m_particles.end());
		m_index.buildIndex();
	}
};
