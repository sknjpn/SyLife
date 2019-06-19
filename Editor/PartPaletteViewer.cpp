#include "PartPaletteViewer.h"
#include "AssetManager.h"

void PartPaletteViewer::Update()
{
	static s3d::Font font14(13);
	static s3d::Font font12(10, s3d::Typeface::Bold);

	s3d::Vec2 pos(0.0, 0.0);

	// Body
	{
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(pos), true);

			font14(U"Body").draw();
			pos.moveBy(0.0, 16.0);
		}

		pos.moveBy(16.0, 0.0);
		const auto& models = g_assetManagerPtr->m_bodyModels;
		for (auto it = models.begin(); it != models.end(); ++it)
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(pos), true);
			auto f = font12(s3d::Unicode::Widen((*it)->m_name));

			f.region().draw(m_model->m_body == *it ? s3d::Palette::Darkorange : s3d::ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
			f.draw();

			if (f.region().leftClicked()) m_model->m_body = *it;

			pos.moveBy(0.0, 15.0);
		}
		pos.moveBy(-16.0, 0.0);
	}

	// Equipment

	// Module
}
