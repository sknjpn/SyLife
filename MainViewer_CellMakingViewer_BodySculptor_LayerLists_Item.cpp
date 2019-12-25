#include "MainViewer.h"
#include "Layer.h"

MainViewer::CellMakingViewer::BodySculptor::LayerLists::Item::Item(const shared_ptr<PartAsset>& partAsset)
	: m_partAsset(partAsset)
{
	const int length = 80;

	// Circle
	{
		const Size size(length, length);
		Image image(size);
		for (auto p : step(size))
		{
			const Point d = (p - size / 2);

			if (d.length() < length / 2)
				image[p] = HSV(ToDegrees(atan2(d.y, d.x)), d.length() / (length / 2), 1.0);
			else
				image[p] = Color(0, 0);
		}
		m_circleTexture = Texture(image);
	}

	// Bar
	{
		const Size size(length / 4, length);
		Image image(size);
		for (auto p : step(size))
		{
			image[p] = ColorF(1.0 - p.y / double(length));
		}
		m_barTexture = Texture(image);
	}
}

void MainViewer::CellMakingViewer::BodySculptor::LayerLists::Item::update()
{
	const int length = 64;

	// BackGround
	{
		const auto block = RectF(getViewerSize()).stretched(-2.0);
		block.draw(m_isSelected ? ColorF(Palette::Blue, 0.5) : ColorF(1.0, block.mouseOver() ? 0.5 : 0.25)).drawFrame(1.0, Palette::White);
	}

	// Circle
	{
		auto t = Transformer2D(Mat3x2::Translate(10, 10), true);

		m_circleTexture.draw();

		RectF(Vec2(10, 10))
			.setCenter(Vec2(m_hsv.s * length / 2.0, 0.0).rotated(ToRadians(m_hsv.h)))
			.movedBy(length / 2, length / 2)
			.drawFrame(1.0, Palette::Black);

		auto d = Cursor::PosF() - Vec2(length / 2.0, length / 2.0);
		if (Circle(Vec2(32, 32), 32).leftClicked()) m_circleSelected = true;
		if (m_circleSelected)
		{
			m_hsv.h = ToDegrees(atan2(d.y, d.x));
			m_hsv.s = Min(1.0, d.length() / (length / 2));
		}
	}

	// Bar
	{
		auto t = Transformer2D(Mat3x2::Translate(120, 10), true);

		m_barTexture.draw();

		Triangle(Vec2(0, 0), Vec2(8, -4), Vec2(8, 4))
			.movedBy(length / 4, (1.0 - m_hsv.v) * length)
			.draw()
			.drawFrame(1.0, Palette::Black);

		if (Rect(length / 4, length).leftClicked()) m_barSelected = true;
		if (m_barSelected) m_hsv.v = Clamp<double>(1.0 - Cursor::PosF().y / length, 0.0, 1.0);	// 完全に0にするとSも0になるので注意
	}

	if (MouseL.up())
	{
		m_barSelected = false;
		m_circleSelected = false;
	}
}
