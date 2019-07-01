#include "SystemManager.h"
#include "ViewerManager.h"
#include "TitleViewer.h"

void Main()
{
	Window::SetTitle(U"SyLife");
	
	Graphics::SetFullScreen(true, Size(1920, 1080));
	Window::SetBaseSize(Size(1920, 1080));

	//Window::Resize(Size(1920, 1080));

	g_systemManagerPtr = make_unique<SystemManager>();

	g_viewerManagerPtr->AddViewer<TitleViewer>();

	while (System::Update())
	{
		g_systemManagerPtr->Update();

		if (KeyEscape.down()) break;
	}
}
