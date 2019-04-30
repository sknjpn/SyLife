#include "CellManager.h"
#include "Cell.h"
#include "FieldManager.h"
#include "RigidbodySearcher.h"
#include "Molecule.h"
#include "MoleculeManager.h"

unique_ptr<CellManager>	g_cellManager;

CellManager::CellManager()
{
	m_cells.reserve(10000);
}


CellManager::~CellManager()
{
}

const shared_ptr<Cell>& CellManager::AddCell()
{
	const auto& c = m_cells.emplace_back(make_shared<Cell>());

	g_fieldManager->m_rigidbodies.emplace_back(c);

	return c;
}

void CellManager::Update()
{
	for (const auto& c : m_cells)
	{
		auto list = g_rigidbodySearcher->GetNearRigidbodies(c->m_position, c->m_radius * 2.0);

		for (const auto& l : list)
		{
			auto t = g_fieldManager->m_rigidbodies[l.first];
			auto length = (t->m_position - c->m_position).length();

			if (t != c && length - t->m_radius - c->m_radius < 0.0)
			{
				auto m = dynamic_pointer_cast<Molecule>(t);

				if (m != nullptr)
				{
					{
						auto it = find(g_moleculeManager->m_molecules.begin(), g_moleculeManager->m_molecules.end(), m);
						g_moleculeManager->m_molecules.erase(it);
					}
					{
						auto it = find(g_fieldManager->m_rigidbodies.begin(), g_fieldManager->m_rigidbodies.end(), m);
						g_fieldManager->m_rigidbodies.erase(it);
					}
				}

				g_rigidbodySearcher->m_index.buildIndex();

				break;
			}
		}
	}
}
