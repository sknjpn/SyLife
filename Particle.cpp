#include "Particle.h"
#include "ChipManager.h"
#include "Chip.h"

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
	if (m_position.x > g_chipManagerPtr->getFieldSize().x) m_position.x = g_chipManagerPtr->getFieldSize().x;
	if (m_position.y > g_chipManagerPtr->getFieldSize().y) m_position.y = g_chipManagerPtr->getFieldSize().y;

	// 水流
	{
		const double scale = 100.0;
		const auto waveVelocity = scale * g_chipManagerPtr->getChip(m_position)->getWaveVelocity();

		m_velocity = waveVelocity + (m_velocity - waveVelocity) * 0.95;
	}
}