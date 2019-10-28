#include "CreateCellButton.h"

CreateCellButton::CreateCellButton()
{
	setViewerRect(RectF(200, 50).setCenter(Scene::CenterF().x, 50));
	setBackgroundColor(Color(0, 0));
}

void CreateCellButton::update()
{
	static bool isClicked = false;
	RectF rect(getViewerRect().size);

	ColorF color = isClicked ? ColorF(1.0) : isMouseOver() ? ColorF(0.75) : ColorF(0.5);

	rect.rounded(5).draw(color).drawFrame(2.0, 0.0, Palette::Black);

	static Font font(32, Typeface::Bold);
	font(U"Cellの作成").drawAt(rect.center(), Palette::Black);

	if (isMouseOver() && MouseL.down()) isClicked = true;
	if (isMouseOver() && MouseL.up() && isClicked)
	{
		// 起動
	}
}

