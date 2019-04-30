#pragma once

struct RigidbodyCloud
{
	size_t kdtree_get_point_count() const;

	double kdtree_get_pt(const size_t idx, const size_t dim) const;

	template <class BBOX>
	bool kdtree_get_bbox(BBOX&) const { return false; }
};

typedef KDTreeSingleIndexAdaptor<L2_Simple_Adaptor<double, RigidbodyCloud>, RigidbodyCloud, 2> RigidbodyAdaptor;