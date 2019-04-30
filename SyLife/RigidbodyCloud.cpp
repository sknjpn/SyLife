#include "RigidbodyCloud.h"
#include "Rigidbody.h"
#include "FieldManager.h"

size_t RigidbodyCloud::kdtree_get_point_count() const
{
	return g_fieldManagerPtr->m_rigidbodies.size();
}

double RigidbodyCloud::kdtree_get_pt(const size_t idx, const size_t dim) const
{
	return (dim == 0) ? g_fieldManagerPtr->m_rigidbodies[idx]->m_position.m_x : g_fieldManagerPtr->m_rigidbodies[idx]->m_position.m_y;
}
