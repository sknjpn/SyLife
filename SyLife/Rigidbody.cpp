#include "Rigidbody.h"
#include "FieldManager.h"

void Rigidbody::AddImpulseInWorld(const Vec2& impulse, const Vec2& worldPosition)
{
	// ‰^“®—Ê
	auto momentum = GetVelocity() * GetMass() + impulse;

	// ‘¬“x
	SetVelocity(momentum / GetMass());

	// —Í‚Ìƒ‚[ƒƒ“ƒg
	auto momentOfForce = (worldPosition - GetPosition()).cross(impulse);

	// Šp‰^“®—Ê
	auto angularMomentum = momentOfForce + m_inertia * m_angularVelocity;

	// Šp‘¬“x
	m_angularVelocity = angularMomentum / m_inertia;
}

void Rigidbody::UpdateRigidbody()
{
	// Õ“Ëˆ—
	g_fieldManagerPtr->GetIndexer().ForEachNearParticles(GetPosition(), m_radius * 2.0, [this](const auto& p, double distance) {
		auto t = dynamic_pointer_cast<Rigidbody>(p);

		distance = sqrt(distance);
		if (t->GetPosition() != GetPosition() && distance - t->m_radius - m_radius < 0.0)
		{
			if (this == t.get()) return;

			auto f = -10000.0 * (t->GetPosition() - GetPosition()).normalized();
			AddForceInWorld(f, GetPosition());
			t->AddForceInWorld(-f, t->GetPosition());
		}
		}
	);

	// ‰ñ“]‰^“®
	m_rotation += m_angularVelocity * g_fieldManagerPtr->GetDeltaTime();

	// –€C’ïR
	m_angularVelocity *= 0.95;

	UpdateParticle();
}
