#include "ParticleCloud.h"
#include "Particle.h"
#include "FieldManager.h"

size_t ParticleCloud::kdtree_get_point_count() const
{
	return g_fieldManagerPtr->m_rigidbodies.size();
}

double ParticleCloud::kdtree_get_pt(const size_t idx, const size_t dim) const
{
	return (dim == 0) ? m_particles[idx]->m_position.m_x : m_particles[idx]->m_position.m_y;
}
