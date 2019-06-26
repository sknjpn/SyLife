#include "FieldViewer.h"
#include "FieldManager.h"
#include "Curtain.h"

void FieldViewer::Init()
{
}

void FieldViewer::Update(bool isMouseOver)
{
	if (isMouseOver) m_cursorCamera2D.update();
	auto t = m_cursorCamera2D.createTransformer();

	auto f = s3d::KeySpace.pressed() ? 5 : 1;

	for (int i = 0; i < f; ++i)
		g_fieldManagerPtr->Update();

	g_fieldManagerPtr->Draw();

	static Curtain curtain(s3d::Color(11, 22, 33), 0.5);
	curtain.OpenUpdate();
	m_audio.setVolume(s3d::Min(curtain.m_st.sF() / curtain.m_duration, 1.0));
}
