#include "Molecule.h"

#include "ViewerManager.h"

inline void MoleculeModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<MoleculeViewer>()->SetModel(shared_from_this());
}
