#pragma once

#include "ParticleCloud.h"

class Particle;

class ParticleSearcher
{
public:
	ParticleCloud	m_cloud;
	ParticleAdaptor	m_index;

public:
	ParticleSearcher();
	~ParticleSearcher();

	void	ForEachNearParticles(Vector2D position, double radius, function<void(const shared_ptr<Particle>&, double)> func) const;

	std::vector<std::pair<size_t, double>>	GetNearParticles(Vector2D position, double radius) const;
};

extern unique_ptr<ParticleSearcher> g_particleSearcherPtr;