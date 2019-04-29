#pragma once

#include "nanoflann.hpp"

class Rigidbody {

public:
	double	m_mass = 0.0;
	double	m_radius = 0.0;
	Vector2D	m_position;
	Vector2D	m_velocity;
	double	m_rotation = 0.0;
	double	m_inertia = 0.0;
	double	m_angularVelocity = 0.0;

public:
	Rigidbody() = default;
	virtual ~Rigidbody() = default;

	Vector2D	GetVelocity(const Vector2D& position) const;

	const Vector2D&	GetWorldPosition(const Vector2D& localPosition) const { return m_position + localPosition.rotated(m_rotation); }

	void	AddForceInWorld(const Vector2D& force, const Vector2D& worldPosition);
	void	AddForceInLocal(const Vector2D& force, const Vector2D& localPosition) { AddForceInWorld(force.rotated(m_rotation), GetWorldPosition(localPosition)); }

	void	AddImpulseInWorld(const Vector2D& impulse, const Vector2D& worldPosition);
	void	AddImpulseInLocal(const Vector2D& impulse, const Vector2D& localPosition) { AddImpulseInWorld(impulse.rotated(m_rotation), GetWorldPosition(localPosition)); }
};

struct RigidbodyCloud
{
	vector<shared_ptr<Rigidbody>>	m_rigidbodies;

	inline size_t kdtree_get_point_count() const { return m_rigidbodies.size(); }

	inline double kdtree_get_pt(const size_t idx, const size_t dim) const
	{
		return (dim == 0) ? m_rigidbodies[idx]->m_position.m_x : m_rigidbodies[idx]->m_position.m_y;
	}
};

typedef KDTreeSingleIndexAdaptor<L2_Simple_Adaptor<double, RigidbodyCloud>, RigidbodyCloud, 2> RigidbodyAdaptor;