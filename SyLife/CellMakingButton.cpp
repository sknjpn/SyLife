#include "CellMakingButton.h"
#include "CellMakingViewer.h"

void CellMakingButton::onPushed()
{
	destroy();
	getParentViewer()->addChildViewer<CellMakingViewer>();
}

CellMakingButton::CellMakingButton()
{
	setViewerRect(RectF(200, 50).setCenter(Scene::CenterF().x, 50));
	setBackgroundColor(Color(0, 0));
}

void CellMakingButton::update()
{
	RectF rect(getViewerRect().size);

	ColorF color = (m_isClicked && isMouseover()) ? ColorF(1.0) : isMouseover() ? ColorF(0.75) : ColorF(0.5);

	rect.rounded(5).draw(color).drawFrame(2.0, 0.0, Palette::Black);

	static Font font(32, Typeface::Bold);
	font(U"Cellの作成").drawAt(rect.center(), Palette::Black);

	if (isMouseover() && MouseL.down()) m_isClicked = true;
	if (isMouseover() && MouseL.up() && m_isClicked) m_isSelected = true;
	if (MouseL.up()) m_isClicked = false;
}

