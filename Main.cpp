#include "WorldGenerateViewer.h"
#include "World.h"

void Main()
{
	Window::SetTitle(U"SyLife");
	Window::Resize(1920, 1080);
	Window::SetStyle(WindowStyle::Sizable);
	Window::Maximize();

	Viewer::GetRootViewer()->addChildViewer<WorldGenerateViewer>();
	Viewer::Run();
}
