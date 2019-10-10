#include "Particle.h"

#include "SystemManager.h"
#include "WaveManager.h"
#include "FieldManager.h"

void Particle::addForce(const Vec2& force)
{
	m_velocity += force / m_mass * g_systemManagerPtr->GetDeltaTime();
}

void Particle::updateParticle()
{
	// 並進運動
	m_position += m_velocity * g_systemManagerPtr->GetDeltaTime();

	// 壁
	if (m_position.x < 0) m_position.x = 0;
	if (m_position.y < 0) m_position.y = 0;
	if (m_position.x > g_fieldManagerPtr->getSize().x) m_position.x = g_fieldManagerPtr->getSize().x;
	if (m_position.y > g_fieldManagerPtr->getSize().y) m_position.y = g_fieldManagerPtr->getSize().y;

	if (KeyO.pressed())
	{
		m_velocity *= 0.95;
	}
	else
	{
		// 摩擦抵抗
		auto waveVelocity = g_waveManagerPtr->getWaveVelocity(m_position);
		m_velocity = waveVelocity + (m_velocity - waveVelocity) * 0.95;
	}
}