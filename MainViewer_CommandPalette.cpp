#include "MainViewer.h"

void MainViewer::CommandPalette::init()
{
	setViewerSize(65 * 4 + 5, 70);
	setViewerPosInLocal(Scene::Size().x - getViewerSize().x - 20, 20);
}

void MainViewer::CommandPalette::update()
{
	RectF(getViewerSize()).rounded(5).draw(Palette::White).drawFrame(2.0, 0.0, Palette::Black);

	moveDrawPos(5, 5);

	const auto rect = RectF(60, 60).rounded(5);

	// Zoom In
	{
		const auto colorTex = rect.leftPressed() ? Palette::Red : Palette::Gray;
		rect.draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

		m_textureZoomIn.drawAt(rect.center(), colorTex);

		if (rect.leftPressed()) getParentViewer()->getChildViewer<FieldViewer>()->getCamera().zoomIn();

		moveDrawPos(65, 0);
	}

	// Zoom Out
	{
		const auto colorTex = rect.leftPressed() ? Palette::Red : Palette::Gray;
		rect.draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

		m_textureZoomOut.drawAt(rect.center(), colorTex);

		if (rect.leftPressed()) getParentViewer()->getChildViewer<FieldViewer>()->getCamera().zoomOut();

		moveDrawPos(65, 0);
	}

	// Fast Mode
	{
		const auto colorTex = rect.leftPressed() ? Palette::Red : Palette::Gray;
		rect.draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

		auto& isHighSpeed = getParentViewer()->getChildViewer<FieldViewer>()->m_isHighSpeed;

		m_textureFast.drawAt(rect.center(), isHighSpeed ? Palette::Red : Palette::Gray);

		if (rect.leftClicked()) isHighSpeed = !isHighSpeed;

		moveDrawPos(65, 0);
	}

	// Poison
	{
		const auto colorTex = rect.leftPressed() ? Palette::Red : Palette::Gray;
		rect.draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

		auto& isPoisonEnabled = getParentViewer()->getChildViewer<FieldViewer>()->m_isPoisonEnabled;

		m_texturePoison.drawAt(rect.center(), isPoisonEnabled ? Palette::Red : Palette::Gray);

		if (rect.leftClicked()) isPoisonEnabled = !isPoisonEnabled;

		moveDrawPos(65, 0);
	}
}
