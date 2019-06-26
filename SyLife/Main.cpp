#include "SystemManager.h"
#include "ViewerManager.h"
#include "TitleViewer.h"

void Main()
{
	s3d::Window::SetTitle(U"SyLife");
	s3d::Graphics::SetFullScreen(true, s3d::Size(1920, 1080));
	s3d::Window::SetBaseSize(s3d::Size(1920, 1080));

	g_systemManagerPtr = make_unique<SystemManager>();

	g_viewerManagerPtr->AddViewer<TitleViewer>();

	g_systemManagerPtr->Start();

	while (s3d::System::Update())
	{
		g_systemManagerPtr->Update();
	}

	/*
	s3d::Graphics::SetFullScreen(true, s3d::Size(1920, 1080));
	s3d::Window::SetBaseSize(s3d::Size(1920, 1080));


	RestrictedCamera2D cursorCamera2D;
	g_systemManagerPtr = make_unique<SystemManager>();
	g_fieldManagerPtr->m_size = Vector2D(1920, 1080);

	while (s3d::System::Update())
	{
		cursorCamera2D.update();
		auto t = cursorCamera2D.createTransformer();

		g_systemManagerPtr->Update();
		g_systemManagerPtr->Draw();

		if (s3d::KeyEscape.down()) break;
	}
	*/
}
