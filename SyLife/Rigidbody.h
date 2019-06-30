#pragma once

#include "Particle.h"
#include "nanoflann.hpp"
#include "Rigidbody.h"
#include "FieldManager.h"

#include <boost/math/constants/constants.hpp>
using namespace boost::math::constants;

class Rigidbody 
	: public Particle
{
public:
	double	m_radius = 0.0;
	double	m_rotation = 0.0;
	double	m_inertia = 0.0;
	double	m_angularVelocity = 0.0;

public:
	Rigidbody() = default;
	virtual ~Rigidbody() = default;

	void	Init()
	{
		// äpâ^ìÆó ÇÃåvéZ
		m_inertia = m_mass * m_radius * m_radius * 0.5;
	}

	Vector2D	GetVelocity(const Vector2D& position) const
	{
		auto delta = position - m_position;


		return m_velocity + delta.rotated(pi<double>()) * m_angularVelocity;
	}

	Vector2D	GetWorldPosition(const Vector2D& localPosition) const { return m_position + localPosition.rotated(m_rotation); }

	void	AddForceInWorld(const Vector2D& force, const Vector2D& worldPosition)
	{
		AddImpulseInWorld(force * g_fieldManagerPtr->GetDeltaTime(), worldPosition);
	}
	void	AddForceInLocal(const Vector2D& force, const Vector2D& localPosition) { AddForceInWorld(force.rotated(m_rotation), GetWorldPosition(localPosition)); }

	void	AddImpulseInWorld(const Vector2D& impulse, const Vector2D& worldPosition)
	{
		// â^ìÆó 
		auto momentum = m_velocity * m_mass + impulse;

		// ë¨ìx
		m_velocity = momentum / m_mass;

		// óÕÇÃÉÇÅ[ÉÅÉìÉg
		auto momentOfForce = (worldPosition - m_position).cross(impulse);

		// äpâ^ìÆó 
		auto angularMomentum = momentOfForce + m_inertia * m_angularVelocity;

		// äpë¨ìx
		m_angularVelocity = angularMomentum / m_inertia;
	}
	void	AddImpulseInLocal(const Vector2D& impulse, const Vector2D& localPosition) { AddImpulseInWorld(impulse.rotated(m_rotation), GetWorldPosition(localPosition)); }
};
