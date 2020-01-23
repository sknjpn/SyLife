#include "MainViewer.h"

void MainViewer::CommandPalette::init()
{
	setViewerSize(65 * 4 + 5, 65 * 2 + 10);
	setViewerPosInLocal(Scene::Size().x - getViewerSize().x - 20, 20);
}

void MainViewer::CommandPalette::update()
{
	RectF(getViewerSize()).rounded(5).draw(Palette::White).drawFrame(2.0, 0.0, Palette::Black);

	moveDrawPos(5, 5);

	const auto rect = RectF(60, 60).rounded(5);
	const auto fv = getParentViewer()->getChildViewer<FieldViewer>();

	// Zoom In
	{
		const auto colorTex = rect.leftPressed() ? Palette::Red : Palette::Gray;
		rect.draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

		m_textureZoomIn.drawAt(rect.center(), colorTex);

		if (rect.leftPressed()) fv->getCamera().zoomIn();

		moveDrawPos(65, 0);
	}

	// Zoom Out
	{
		const auto colorTex = rect.leftPressed() ? Palette::Red : Palette::Gray;
		rect.draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

		m_textureZoomOut.drawAt(rect.center(), colorTex);

		if (rect.leftPressed()) fv->getCamera().zoomOut();

		moveDrawPos(65, 0);
	}

	// Fast Mode
	{
		const auto colorTex = rect.leftPressed() ? Palette::Red : Palette::Gray;
		rect.draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

		auto& isHighSpeed = fv->m_isHighSpeed;

		m_textureFast.drawAt(rect.center(), isHighSpeed ? Palette::Red : Palette::Gray);

		if (rect.leftClicked()) isHighSpeed = !isHighSpeed;

		moveDrawPos(65, 0);
	}

	// Mode
	{
		setDrawPos(5, 75);

		RectF(65 * 4 - 5, 65 - 5).stretched(5).rounded(5).draw(ColorF(0.75)).drawFrame(2.0, 0.0, Palette::Black);

		// Hand
		{
			const auto colorTex = rect.leftPressed() ? Palette::Red : Palette::Gray;
			rect.draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

			m_textureHand.drawAt(rect.center(), fv->m_handAction == FieldViewer::HandAction::None ? Palette::Red : Palette::Gray);

			if (rect.leftClicked()) fv->m_handAction = FieldViewer::HandAction::None;

			moveDrawPos(65, 0);
		}

		// AddElement
		{
			const auto colorTex = rect.leftPressed() ? Palette::Red : Palette::Gray;
			rect.draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

			m_textureAddElement.drawAt(rect.center(), fv->m_handAction == FieldViewer::HandAction::AddElement ? Palette::Red : Palette::Gray);

			if (rect.leftClicked()) fv->m_handAction = FieldViewer::HandAction::AddElement;

			moveDrawPos(65, 0);
		}

		// Poison
		{
			const auto colorTex = rect.leftPressed() ? Palette::Red : Palette::Gray;
			rect.draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

			m_texturePoison.drawAt(rect.center(), fv->m_handAction == FieldViewer::HandAction::Poison ? Palette::Red : Palette::Gray);

			if (rect.leftClicked()) fv->m_handAction = FieldViewer::HandAction::Poison;

			moveDrawPos(65, 0);
		}

		// Trash
		{
			const auto colorTex = rect.leftPressed() ? Palette::Red : Palette::Gray;
			rect.draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

			m_textureTrashBox.drawAt(rect.center(), fv->m_handAction == FieldViewer::HandAction::Trash ? Palette::Red : Palette::Gray);

			if (rect.leftClicked()) fv->m_handAction = FieldViewer::HandAction::Trash;

			moveDrawPos(65, 0);
		}
	}
}
