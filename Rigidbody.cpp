#include "Rigidbody.h"

void Rigidbody::addForceInWorld(const Vec2& force, const Vec2& worldPosition)
{
	addImpulseInWorld(force * DeltaTime, worldPosition);
}

void Rigidbody::addImpulseInWorld(const Vec2& impulse, const Vec2& worldPosition)
{
	// 運動量
	const Vec2 momentum = getVelocity() * getMass() + impulse;

	// 速度
	setVelocity(momentum / getMass());

	// 力のモーメント
	const double momentOfForce = (worldPosition - getPosition()).cross(impulse);

	// 角運動量
	const double angularMomentum = momentOfForce + m_inertia * m_angularVelocity;

	// 角速度
	m_angularVelocity = angularMomentum / m_inertia;
}

void Rigidbody::updateRigidbody()
{
	// 回転運動
	m_rotation += m_angularVelocity * DeltaTime;

	// 摩擦抵抗
	m_angularVelocity *= 0.95;
}

void Rigidbody::load(Deserializer<ByteArray>& reader)
{
	Particle::load(reader);

	reader >> m_radius;
	reader >> m_rotation;
	reader >> m_inertia;
	reader >> m_angularVelocity;
}

void Rigidbody::save(Serializer<MemoryWriter>& writer) const
{
	Particle::save(writer);

	writer << m_radius;
	writer << m_rotation;
	writer << m_inertia;
	writer << m_angularVelocity;
}
