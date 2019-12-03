#include "SoundMixerViewer.h"
#include "SoundManager.h"

void SoundMixerViewer::init()
{
	setViewerRect(120, 600, 320, 240);
}

void SoundMixerViewer::update()
{
	RectF(getViewerSize()).draw(ColorF(0.5, 1.0)).drawFrame(2.0, ColorF(1.0, 1.0));

	static Font font(20, Typeface::Bold);

	// Master
	{
		moveDrawPos(5, 0);
		font(U"Master Volume").draw(Vec2::Zero(), Palette::Black);
		moveDrawPos(-5, 24);

		moveDrawPos(10, 0);
		Rect(300, 8).setCenter(150, 16).draw(Palette::Black);
		auto circle = Circle(Vec2(g_soundManagerPtr->getMasterVolume() * 300, 16), 8);
		circle.draw(Palette::Red.lerp(Palette::White, circle.mouseOver() ? 0.5 : 0.0)).drawFrame(1.0, Palette::Black);

		static bool isGrabbed = false;
		if (circle.leftClicked()) isGrabbed = true;
		if (!MouseL.pressed()) isGrabbed = false;

		if (isGrabbed) g_soundManagerPtr->setMasterVolume(Clamp<double>(Cursor::PosF().x / 300.0, 0.0, 1.0));

		moveDrawPos(-10, 32);
	}

	// UI
	{
		moveDrawPos(5, 0);
		font(U"UI Volume").draw(Vec2::Zero(), Palette::Black);
		moveDrawPos(-5, 24);

		moveDrawPos(10, 0);
		Rect(300, 8).setCenter(150, 16).draw(Palette::Black);
		auto circle = Circle(Vec2(g_soundManagerPtr->getUIVolume() * 300, 16), 8);
		circle.draw(Palette::Red.lerp(Palette::White, circle.mouseOver() ? 0.5 : 0.0)).drawFrame(1.0, Palette::Black);

		static bool isGrabbed = false;
		if (circle.leftClicked()) isGrabbed = true;
		if (!MouseL.pressed()) isGrabbed = false;

		if (isGrabbed) g_soundManagerPtr->setUIVolume(Clamp<double>(Cursor::PosF().x / 300.0, 0.0, 1.0));

		moveDrawPos(-10, 32);
	}

	// Environment
	{
		moveDrawPos(5, 0);
		font(U"Environment Volume").draw(Vec2::Zero(), Palette::Black);
		moveDrawPos(-5, 24);

		moveDrawPos(10, 0);
		Rect(300, 8).setCenter(150, 16).draw(Palette::Black);
		auto circle = Circle(Vec2(g_soundManagerPtr->getEnvironmentVolume() * 300, 16), 8);
		circle.draw(Palette::Red.lerp(Palette::White, circle.mouseOver() ? 0.5 : 0.0)).drawFrame(1.0, Palette::Black);

		static bool isGrabbed = false;
		if (circle.leftClicked()) isGrabbed = true;
		if (!MouseL.pressed()) isGrabbed = false;

		if (isGrabbed) g_soundManagerPtr->setEnvironmentVolume(Clamp<double>(Cursor::PosF().x / 300.0, 0.0, 1.0));

		moveDrawPos(-10, 32);
	}

	// Music
	{
		moveDrawPos(5, 0);
		font(U"Music Volume").draw(Vec2::Zero(), Palette::Black);
		moveDrawPos(-5, 24);

		moveDrawPos(10, 0);
		Rect(300, 8).setCenter(150, 16).draw(Palette::Black);
		auto circle = Circle(Vec2(g_soundManagerPtr->getMusicVolume() * 300, 16), 8);
		circle.draw(Palette::Red.lerp(Palette::White, circle.mouseOver() ? 0.5 : 0.0)).drawFrame(1.0, Palette::Black);

		static bool isGrabbed = false;
		if (circle.leftClicked()) isGrabbed = true;
		if (!MouseL.pressed()) isGrabbed = false;

		if (isGrabbed) g_soundManagerPtr->setMusicVolume(Clamp<double>(Cursor::PosF().x / 300.0, 0.0, 1.0));

		moveDrawPos(-10, 32);
	}
}
