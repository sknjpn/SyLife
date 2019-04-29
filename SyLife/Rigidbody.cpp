#include "Rigidbody.h"
#include "FieldManager.h"

void Rigidbody::Init()
{
	// 角運動量の計算
	m_inertia = m_mass * m_radius * m_radius * 0.5;
}

Vector2D Rigidbody::GetVelocity(const Vector2D& position) const
{
	auto delta = position - m_position;


	return m_velocity + delta.rotated(pi) * m_angularVelocity;
}

void Rigidbody::AddForceInWorld(const Vector2D& force, const Vector2D& worldPosition)
{
	AddImpulseInWorld(force * g_fieldManager->GetDeltaTime(), worldPosition);
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

size_t RigidbodyCloud::kdtree_get_point_count() const
{ { return g_fieldManager->m_rigidbodies.size(); }
}

double RigidbodyCloud::kdtree_get_pt(const size_t idx, const size_t dim) const
{
	return (dim == 0) ? g_fieldManager->m_rigidbodies[idx]->m_position.m_x : g_fieldManager->m_rigidbodies[idx]->m_position.m_y;

}
