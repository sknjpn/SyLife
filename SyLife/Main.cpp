#include "SystemManager.h"
#include "ViewerManager.h"
#include "TitleViewer.h"

void Main()
{
	s3d::Window::SetTitle(U"SyLife");
	
	s3d::Graphics::SetFullScreen(true, s3d::Size(1920, 1080));
	s3d::Window::SetBaseSize(s3d::Size(1920, 1080));

	//s3d::Window::Resize(s3d::Size(1920, 1080));

	g_systemManagerPtr = make_unique<SystemManager>();

	g_viewerManagerPtr->AddViewer<TitleViewer>();

	while (s3d::System::Update())
	{
		g_systemManagerPtr->Update();

		if (s3d::KeyEscape.down()) break;
	}
}
