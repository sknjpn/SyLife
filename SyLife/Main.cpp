#include "FieldManager.h"
#include "CellManager.h"
#include "MoleculeManager.h"


void Main()
{
	g_fieldManager = make_unique<FieldManager>();

	g_fieldManager->Init();

	while (s3d::System::Update())
	{
		g_fieldManager->Update();

		// Moleculeの描画

		// Cellの描画
	}
}
