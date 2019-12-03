#include "Particle.h"
#include "ChipState.h"
#include "World.h"

void Particle::addForce(const Vec2& force)
{
	m_velocity += force / m_mass * DeltaTime;
}

void Particle::updateParticle()
{
	// 並進運動
	m_position += m_velocity * DeltaTime;

	// 壁
	if (m_position.x < 0) m_position.x = 0;
	if (m_position.y < 0) m_position.y = 0;
	if (m_position.x > World::GetInstance()->getField().getFieldSize().x) m_position.x = World::GetInstance()->getField().getFieldSize().x;
	if (m_position.y > World::GetInstance()->getField().getFieldSize().y) m_position.y = World::GetInstance()->getField().getFieldSize().y;

	// 水流
	{
		const double scale = 100.0;
		const auto waveVelocity = scale * World::GetInstance()->getField().getChip(m_position)->getWaveVelocity();

		m_velocity = waveVelocity + (m_velocity - waveVelocity) * 0.95;
	}
}

void Particle::load(const JSONValue& json)
{
	m_mass = json[U"mass"].get<double>();
	m_position = json[U"position"].get<Vec2>();
	m_velocity = json[U"velocity"].get<Vec2>();
}

void Particle::save(JSONWriter& json) const
{
	json.key(U"mass").write(m_mass);
	json.key(U"position").write(m_position);
	json.key(U"velocity").write(m_velocity);
}
