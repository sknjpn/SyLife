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
	// Õ“Ëˆ—
	{
		auto result = g_fieldManagerPtr->GetRigidbodyKDTree().knnSearch(2, GetPosition());
		if (result.size() == 2)
		{
			auto& t = g_fieldManagerPtr->GetRigidbody()[result[1]];

			if (t->GetPosition() != GetPosition() && (m_radius + t->m_radius - (t->GetPosition() - GetPosition()).length()) > 0)
			{
				auto f = -1000.0 * (t->GetPosition() - GetPosition()).normalized() * (m_radius + t->m_radius - (t->GetPosition() - GetPosition()).length());
				AddForceInWorld(f, GetPosition());
				t->AddForceInWorld(-f, t->GetPosition());
			}
		}
	}

	// ‰ñ“]‰^“®
	m_rotation += m_angularVelocity * g_fieldManagerPtr->GetDeltaTime();

	// –€C’ïR
	m_angularVelocity *= 0.95;
}
