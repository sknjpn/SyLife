#include "AssetViewer.h"
#include "AssetManager.h"
#include "CellModel.h"
#include "MoleculeModel.h"
#include "PartModel.h"

void AssetViewer::Update()
{
	static s3d::Font font14(13);
	static s3d::Font font12(10, s3d::Typeface::Bold);

	s3d::Vec2 pos(0.0, 0.0);

	// Cell
	{
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(pos), true);

			font14(U"Cell").draw();
			pos.moveBy(0.0, 16.0);
		}

		pos.moveBy(16.0, 0.0);
		const auto& models = g_assetManagerPtr->GetModels<CellModel>();
		for (auto it = models.begin(); it != models.end(); ++it)
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(pos), true);
			auto f = font12(s3d::Unicode::Widen((*it)->m_name));

			f.region().draw(m_selectedModel == *it ? s3d::Palette::Darkorange : s3d::ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
			f.draw();

			if (f.region().leftClicked()) SetSelectedModel(*it);

			pos.moveBy(0.0, 15.0);
		}
		pos.moveBy(-16.0, 0.0);
	}

	// Molecule
	{
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(pos), true);

			font14(U"Molecule").draw();
			pos.moveBy(0.0, 16.0);
		}

		pos.moveBy(16.0, 0.0);
		const auto& models = g_assetManagerPtr->GetModels<MoleculeModel>();
		for (auto it = models.begin(); it != models.end(); ++it)
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(pos), true);
			auto f = font12(s3d::Unicode::Widen((*it)->m_name));

			f.region().draw(m_selectedModel == *it ? s3d::Palette::Darkorange : s3d::ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
			f.draw();

			if (f.region().leftClicked()) SetSelectedModel(*it);

			pos.moveBy(0.0, 15.0);
		}
		pos.moveBy(-16.0, 0.0);
	}

	// Part
	{
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(pos), true);

			font14(U"Part").draw();
			pos.moveBy(0.0, 16.0);
		}

		pos.moveBy(16.0, 0.0);
		const auto& models = g_assetManagerPtr->GetModels<PartModel>();
		for (auto it = models.begin(); it != models.end(); ++it)
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(pos), true);
			auto f = font12(s3d::Unicode::Widen((*it)->m_name));

			f.region().draw(m_selectedModel == *it ? s3d::Palette::Darkorange : s3d::ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
			f.draw();

			if (f.region().leftClicked()) SetSelectedModel(*it);

			pos.moveBy(0.0, 15.0);
		}
		pos.moveBy(-16.0, 0.0);
	}

	// Save
	if (s3d::KeyControl.pressed() && s3d::KeyS.down() && m_selectedModel != nullptr)
	{
		write_json(m_selectedModel->GetFilepath(), m_selectedModel->Save());
	}
}
