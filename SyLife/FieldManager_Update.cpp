#include "FieldManager.h"
#include "CellManager.h"
#include "EggManager.h"
#include "MoleculeManager.h"
#include "WaveManager.h"
#include "Rigidbody.h"

void FieldManager::Update()
{
	for (const auto& r : m_indexer.GetParticles())
	{
		m_indexer.ForEachNearParticles(r->m_position, r->m_radius * 2.0, [r](const auto& p, double distance) {
			auto t = dynamic_pointer_cast<Rigidbody>(p);

			distance = sqrt(distance);
			if (t->m_position != r->m_position && distance - t->m_radius - r->m_radius < 0.0)
			{
				if (r->m_radius < t->m_radius) return;
				if (r->m_radius == t->m_radius && r < t) return;

				auto f = 64.0 * (distance - t->m_radius - r->m_radius) * (t->m_position - r->m_position) / distance;
				r->AddForceInWorld(f, r->m_position);
				t->AddForceInWorld(-f, t->m_position);
			}
			});

		// ���i�^��
		r->m_position += r->m_velocity;

		// ��]�^��
		r->m_rotation += r->m_angularVelocity;

		// �ǂł̔���
		if (r->m_position.m_x < 0 && r->m_velocity.m_x < 0) r->m_velocity.m_x = -r->m_velocity.m_x;
		if (r->m_position.m_y < 0 && r->m_velocity.m_y < 0) r->m_velocity.m_y = -r->m_velocity.m_y;
		if (r->m_position.m_x > m_size.m_x && r->m_velocity.m_x > 0) r->m_velocity.m_x = -r->m_velocity.m_x;
		if (r->m_position.m_y > m_size.m_y && r->m_velocity.m_y > 0) r->m_velocity.m_y = -r->m_velocity.m_y;

		// ���C��R
		{
			auto waveVelocity = g_waveManagerPtr->GetWaveVelocity(r->m_position);

			r->m_velocity = waveVelocity + (r->m_velocity - waveVelocity) / (1.0 + r->m_radius * 0.01);

			r->m_angularVelocity /= (1.0 + r->m_radius * 0.01);
		}
	}

	g_eggManagerPtr->Update();
	g_cellManagerPtr->Update();
	g_moleculeManagerPtr->Update();
	g_waveManagerPtr->Update();

	m_indexer.Update();
}
