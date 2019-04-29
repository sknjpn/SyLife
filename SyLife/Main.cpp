#include "FieldManager.h"

void Main()
{
	g_fieldManager = make_unique<FieldManager>();

	g_fieldManager->Init();

	while (s3d::System::Update())
	{
		g_fieldManager->Update();
	}
}
