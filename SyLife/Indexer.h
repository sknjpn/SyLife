#pragma once

#include "Cloud.h"

template<typename T>
class Indexer
{
public:
	Cloud<T>	m_cloud;
	Adaptor<T>	m_adaptor;

public:
	Indexer() : m_adaptor(2, m_cloud, KDTreeSingleIndexAdaptorParams(10000)) { m_cloud.m_particles.reserve(10000); }

	void	ForEachNearParticles(Vector2D position, double radius, function<void(const shared_ptr<T>&, double)> func) const
	{
		for (const auto& l : GetNearParticleIDs(position, radius)) func(m_cloud.m_particles[l.first], l.second);
	}

	vector<pair<size_t, double>>	GetNearParticleIDs(Vector2D position, double radius) const
	{
		const double query_pt[2] = { position.m_x, position.m_y };
		const double search_radius = radius * radius;
		vector<pair<size_t, double>>   ret_matches;
		nanoflann::SearchParams params;
		params.sorted = false;	//ÇÊÇËëÅÇ≠
		const size_t nMatches = m_adaptor.radiusSearch(&query_pt[0], search_radius, ret_matches, params);

		return ret_matches;
	}

	vector<shared_ptr<T>>	GetNearParticles(Vector2D position, double radius) const
	{
		vector<shared_ptr<T>>	result;

		for (const auto& id : GetNearParticleIDs(position, radius)) result.emplace_back(m_cloud.m_particles[id.first]);

		return result;
	}

	const vector<shared_ptr<T>>&	GetParticles() const { return m_cloud.m_particles; }
	void	AddParticle(const shared_ptr<T>& t) { m_cloud.m_particles.emplace_back(t); }
	
	void	Update() 
	{
		m_cloud.m_particles.erase(remove_if(m_cloud.m_particles.begin(), m_cloud.m_particles.end(), [](const auto& r) { return r->IsDestroyed(); }), m_cloud.m_particles.end());
		m_adaptor.buildIndex();
	}
};
