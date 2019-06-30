#include "Particle.h"
#include "WaveManager.h"
#include "FieldManager.h"

void Particle::UpdateParticle()
{
	// •Ài‰^“®
	m_position += m_velocity * g_fieldManagerPtr->GetDeltaTime();

	// –€ŽC’ïR
	auto waveVelocity = g_waveManagerPtr->GetWaveVelocity(m_position);
	m_velocity = waveVelocity + (m_velocity - waveVelocity) * 0.95;
}