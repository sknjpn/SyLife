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

void Rigidbody::load(const JSONValue& json)
{
	Particle::load(json);

	m_radius = json[U"radius"].get<double>();
	m_rotation = json[U"rotation"].get<double>();
	m_inertia = json[U"inertia"].get<double>();
	m_angularVelocity = json[U"angularVelocity"].get<double>();
}

void Rigidbody::save(JSONWriter& json) const
{
	Particle::save(json);

	json.key(U"radius").write(m_radius);
	json.key(U"rotation").write(m_rotation);
	json.key(U"inertia").write(m_inertia);
	json.key(U"angularVelocity").write(m_angularVelocity);
}
