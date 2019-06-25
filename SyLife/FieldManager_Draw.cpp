#include "FieldManager.h"
#include "MoleculeManager.h"
#include "CellManager.h"

void FieldManager::Draw()
{
	// Rigidbody Capture
	{
		static shared_ptr<Rigidbody> selectedRigidbody = nullptr;

		if (s3d::MouseL.down())
		{
			Vector2D cursorPos(s3d::Cursor::PosF().x, s3d::Cursor::PosF().y);

			for (auto target : m_indexer.GetNearParticles(cursorPos, 100))
			{
				if (target->m_radius > (target->m_position - cursorPos).length()) selectedRigidbody = target;
			}
		}

		if (s3d::MouseL.pressed())
		{
			if (selectedRigidbody != nullptr) selectedRigidbody->m_position = Vector2D(s3d::Cursor::PosF().x, s3d::Cursor::PosF().y);
		}
		else selectedRigidbody = nullptr;
	}

	g_moleculeManagerPtr->Draw();

	g_cellManagerPtr->Draw();
}