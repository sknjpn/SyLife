#pragma once

class Particle;

struct ParticleCloud
{
	vector<shared_ptr<Particle>>	m_particles;

public:
	size_t kdtree_get_point_count() const;

	double kdtree_get_pt(const size_t idx, const size_t dim) const;

	template <class BBOX>
	bool kdtree_get_bbox(BBOX&) const { return false; }
};

typedef KDTreeSingleIndexAdaptor<L2_Simple_Adaptor<double, ParticleCloud>, ParticleCloud, 2> ParticleAdaptor;