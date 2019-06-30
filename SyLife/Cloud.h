#pragma once

template<typename T>
class Cloud
{
public:
	vector<shared_ptr<T>>	m_particles;

public:
	size_t kdtree_get_point_count() const { return m_particles.size(); }
	double kdtree_get_pt(const size_t idx, const size_t dim) const { return (dim == 0) ? m_particles[idx]->GetPosition().m_x : m_particles[idx]->GetPosition().m_y; }

	template <class BBOX>
	bool kdtree_get_bbox(BBOX&) const { return false; }
};

template<typename T>
using Adaptor = KDTreeSingleIndexAdaptor<L2_Simple_Adaptor<double, Cloud<T>>, Cloud<T>, 2>;
