#include "ParticleSearcher.h"
#include "FieldManager.h"
#include "nanoflann.hpp"

ParticleSearcher::ParticleSearcher()
	: m_index(2, m_cloud, KDTreeSingleIndexAdaptorParams(10000))
{

}

ParticleSearcher::~ParticleSearcher()
{

}

void ParticleSearcher::ForEachNearParticles(Vector2D position, double radius, function<void(const shared_ptr<Particle>&, double)> func) const
{
	for (const auto& l : GetNearParticles(position, radius)) func(m_cloud.m_particles[l.first], l.second);
}

std::vector<std::pair<size_t, double>>  ParticleSearcher::GetNearParticles(Vector2D position, double radius) const
{
	const double query_pt[2] = { position.m_x, position.m_y };
	const double search_radius = radius * radius;
	std::vector<std::pair<size_t, double>>   ret_matches;
	nanoflann::SearchParams params;
	params.sorted = false;	//ÇÊÇËëÅÇ≠
	const size_t nMatches = m_index.radiusSearch(&query_pt[0], search_radius, ret_matches, params);

	return ret_matches;
}

unique_ptr<ParticleSearcher> g_particleSearcherPtr = nullptr;
