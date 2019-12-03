#include "SetupViewer.h"
#include "SoundManager.h"
#include "World.h"

void Main()
{
	Window::SetTitle(U"SyLife");

	g_soundManagerPtr = MakeUnique<SoundManager>();

	Viewer::GetRootViewer()->addChildViewer<SetupViewer>();
	Viewer::Run();

	World::GetInstance()->save();
}
