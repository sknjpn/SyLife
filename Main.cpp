#include "SetupViewer.h"
#include "World.h"

void Main()
{
	Window::SetTitle(U"SyLife");

	Viewer::GetRootViewer()->addChildViewer<SetupViewer>();
	Viewer::Run();
}
