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

	for (int i = 0; i < 4; ++i)
	{
		const auto rect = RectF(60, 60).rounded(5);
		const auto colorTex = rect.leftPressed() ? Palette::Orange : Palette::Gray;

		rect.draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

		switch (i)
		{
		case 0:
		{
			m_textureZoomIn.drawAt(rect.center(), colorTex);

			if (rect.leftPressed()) getParentViewer()->getChildViewer<FieldViewer>()->getCamera().zoomIn();
		}
		break;
		case 1:
		{
			m_textureZoomOut.drawAt(rect.center(), colorTex);

			if (rect.leftPressed()) getParentViewer()->getChildViewer<FieldViewer>()->getCamera().zoomOut();
		}
		break;
		case 2:
		{
			auto& isHighSpeed = getParentViewer()->getChildViewer<FieldViewer>()->m_isHighSpeed;

			m_textureFast.drawAt(rect.center(), isHighSpeed ? Palette::Orange : Palette::Gray);

			if (rect.leftClicked()) isHighSpeed = !isHighSpeed;
		}
		break;
		case 3:
		{
			m_texturePoison.drawAt(rect.center(), colorTex);
		}
		break;
		}

		moveDrawPos(65, 0);
	}
}
