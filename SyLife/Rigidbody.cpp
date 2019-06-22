﻿#include "Rigidbody.h"
#include "FieldManager.h"

#include <boost/math/constants/constants.hpp>
using namespace boost::math::constants;

void Rigidbody::Init()
{
	// 角運動量の計算
	m_inertia = m_mass * m_radius * m_radius * 0.5;
}

Vector2D Rigidbody::GetVelocity(const Vector2D& position) const
{
	auto delta = position - m_position;


	return m_velocity + delta.rotated(pi<double>()) * m_angularVelocity;
}

void Rigidbody::AddForceInWorld(const Vector2D& force, const Vector2D& worldPosition)
{
	AddImpulseInWorld(force * g_fieldManagerPtr->GetDeltaTime(), worldPosition);
}

void Rigidbody::AddImpulseInWorld(const Vector2D& impulse, const Vector2D& worldPosition)
{
	// 運動量
	auto momentum = m_velocity * m_mass + impulse;

	// 速度
	m_velocity = momentum / m_mass;

	// 力のモーメント
	auto momentOfForce = (worldPosition - m_position).cross(impulse);

	// 角運動量
	auto angularMomentum = momentOfForce + m_inertia * m_angularVelocity;

	// 角速度
	m_angularVelocity = angularMomentum / m_inertia;
}