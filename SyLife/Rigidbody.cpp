#include "Rigidbody.h"

void Rigidbody::AddImpulseInWorld(const Vec2& impulse, const Vec2& worldPosition)
{
	// ‰^“®—Ê
	const Vec2 momentum = GetVelocity() * GetMass() + impulse;

	// ‘¬“x
	SetVelocity(momentum / GetMass());

	// —Í‚Ìƒ‚[ƒƒ“ƒg
	const double momentOfForce = (worldPosition - GetPosition()).cross(impulse);

	// Šp‰^“®—Ê
	const double angularMomentum = momentOfForce + m_inertia * m_angularVelocity;

	// Šp‘¬“x
	m_angularVelocity = angularMomentum / m_inertia;
}

void Rigidbody::UpdateRigidbody()
{
	// ‰ñ“]‰^“®
	m_rotation += m_angularVelocity * g_systemManagerPtr->GetDeltaTime();

	// –€C’ïR
	m_angularVelocity *= 0.95;
}
