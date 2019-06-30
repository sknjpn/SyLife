#pragma once

#include "Viewer.h"

#include "Cell.h"
#include "Part.h"

#include "AssetManager.h"

#include "Body.h"
#include "Equipment.h"
#include "Module.h"

class PartPaletteViewer
	: public Viewer
{
public:
	shared_ptr<PartModel>	m_selectedPart;
	shared_ptr<CellModel>	m_model;

	s3d::Vec2	m_drawPos;

public:
	PartPaletteViewer()
		: m_model(make_shared<CellModel>())
	{
		m_drawRect = s3d::RectF(s3d::Window::Width() - 100, 20, 80, 400);
	}
	PartPaletteViewer(const shared_ptr<CellModel>& model)
		: m_model(model)
	{
		m_drawRect = s3d::RectF(s3d::Window::Width() - 100, 20, 80, 400);
	}

	template <typename T>
	void	DrawModels(const string& name)
	{
		static s3d::Font font13(12, s3d::Typeface::Bold);
		static s3d::Font font10(10, s3d::Typeface::Bold);

		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(m_drawPos), true);

			font13(s3d::Unicode::Widen(name)).draw();
			m_drawPos.moveBy(0.0, 16.0);
		}

		m_drawPos.moveBy(16.0, 0.0);
		const auto& models = g_assetManagerPtr->GetModels<T>();
		for (auto it = models.begin(); it != models.end(); ++it)
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(m_drawPos), true);
			auto f = font10(s3d::Unicode::Widen((*it)->GetName()));

			f.region().draw(s3d::ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
			f.draw();

			if (f.region().leftClicked()) m_selectedPart = *it;

			m_drawPos.moveBy(0.0, 15.0);
		}
		m_drawPos.moveBy(-16.0, 0.0);
	}

	void	Update(bool isMouseOver) override
	{
		s3d::Rect(m_drawRect.size.asPoint()).draw(s3d::Color(11, 22, 33, 192));

		m_drawPos = s3d::Vec2::Zero();

		DrawModels<BodyModel>("BodyModel");
		DrawModels<EquipmentModel>("EquipmentModel");
		DrawModels<ModuleModel>("ModuleModel");
	}
};

