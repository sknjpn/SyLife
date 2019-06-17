#include "AssetViewer.h"
#include "AssetManager.h"

void AssetViewer::Update()
{
	static s3d::Font font14(14);
	static s3d::Font font12(12);

	s3d::Vec2 pos(0.0, 0.0);

	// Cell
	{
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(pos), true);

			font14(U"Cell").draw();
			pos.moveBy(0.0, 16.0);
		}

		pos.moveBy(16.0, 0.0);
		const auto& models = g_assetManagerPtr->m_cellModels;
		for (auto it = models.begin(); it != models.end(); ++it)
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(pos), true);
			auto f = font12(s3d::Unicode::Widen((*it)->m_name));

			f.region().draw(s3d::ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
			f.draw();

			pos.moveBy(0.0, 14.0);
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
		const auto& models = g_assetManagerPtr->m_moleculeModels;
		for (auto it = models.begin(); it != models.end(); ++it)
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(pos), true);

			font12(s3d::Unicode::Widen((*it)->m_name)).draw();
			pos.moveBy(0.0, 14.0);
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
		const auto& models = g_assetManagerPtr->m_partModels;
		for (auto it = models.begin(); it != models.end(); ++it)
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(pos), true);

			font12(s3d::Unicode::Widen((*it)->m_name)).draw();
			pos.moveBy(0.0, 14.0);
		}
		pos.moveBy(-16.0, 0.0);
	}
}
