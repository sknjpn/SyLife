#include "FieldManager.h"
#include "SystemManager.h"
#include "CellManager.h"
#include "MoleculeManager.h"
#include "AssetManager.h"
#include "Cell.h"
#include "Molecule.h"

void Main()
{
	g_systemManagerPtr = make_unique<SystemManager>();

	while (s3d::System::Update())
	{
		g_systemManagerPtr->Update();
		g_systemManagerPtr->Draw();
	}
}
