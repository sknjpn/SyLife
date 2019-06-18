#include "AssetViewer.h"
#include "AssetManager.h"
#include "ViewerManager.h"

#include "BodyModel.h"
#include "BodyPropertyViewer.h"
#include "EquipmentModel.h"
#include "EquipmentPropertyViewer.h"
#include "ModuleModel.h"
#include "ModulePropertyViewer.h"
#include "PartShapeViewer.h"

#include "MoleculeModel.h"
#include "MoleculeViewer.h"

#include "CellModel.h"
#include "CellViewer.h"

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
		const auto& models = g_assetManagerPtr->m_cellModels;
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
		const auto& models = g_assetManagerPtr->m_moleculeModels;
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
		const auto& models = g_assetManagerPtr->m_partModels;
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
}

void AssetViewer::SetSelectedModel(const shared_ptr<Model>& model)
{
	// ©•ªˆÈŠO‚ÌView‚ğE‚·
	while (g_viewerManagerPtr->m_viewers.size() > 1) g_viewerManagerPtr->m_viewers.pop_back();

	// ‘ÎÛ‚ÌView‚ğ’Ç‰Á
	if (dynamic_pointer_cast<CellModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<CellViewer>(dynamic_pointer_cast<CellModel>(model));
	if (dynamic_pointer_cast<MoleculeModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<MoleculeViewer>(dynamic_pointer_cast<MoleculeModel>(model));
	if (dynamic_pointer_cast<BodyModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<BodyPropertyViewer>(dynamic_pointer_cast<BodyModel>(model));
	if (dynamic_pointer_cast<EquipmentModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<EquipmentPropertyViewer>(dynamic_pointer_cast<EquipmentModel>(model));
	if (dynamic_pointer_cast<ModuleModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<ModulePropertyViewer>(dynamic_pointer_cast<ModuleModel>(model));
	if (dynamic_pointer_cast<PartModel>(model) != nullptr) g_viewerManagerPtr->AddViewer<PartShapeViewer>(dynamic_pointer_cast<PartModel>(model));
	
	// ‘ÎÛƒ‚ƒfƒ‹‚Ìİ’è
	m_selectedModel = model;
}