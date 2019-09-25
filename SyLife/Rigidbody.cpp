#include "Rigidbody.h"

void Rigidbody::AddImpulseInWorld(const Vec2& impulse, const Vec2& worldPosition)
{
	// 運動量
	const Vec2 momentum = GetVelocity() * GetMass() + impulse;

	// 速度
	SetVelocity(momentum / GetMass());

	// 力のモーメント
	const double momentOfForce = (worldPosition - GetPosition()).cross(impulse);

	// 角運動量
	const double angularMomentum = momentOfForce + m_inertia * m_angularVelocity;

	// 角速度
	m_angularVelocity = angularMomentum / m_inertia;
}

void Rigidbody::UpdateRigidbody()
{
	// 回転運動
	m_rotation += m_angularVelocity * g_systemManagerPtr->GetDeltaTime();

	// 摩擦抵抗
	m_angularVelocity *= 0.95;
}
