#include "FieldViewer.h"
#include "FieldManager.h"

void FieldViewer::Init()
{
}

void FieldViewer::Update()
{
	m_cursorCamera2D.update();
	auto t = m_cursorCamera2D.createTransformer();

	auto f = s3d::KeySpace.pressed() ? 5 : 1;

	for (int i = 0; i < f; ++i)
		g_fieldManagerPtr->Update();

	g_fieldManagerPtr->Draw();
}
