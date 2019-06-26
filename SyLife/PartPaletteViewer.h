#pragma once

#include "Viewer.h"

#include "Cell.h"
#include "Part.h"

#include "AssetManager.h"

class PartPaletteViewer
	: public Viewer
{
public:
	shared_ptr<CellModel>	m_model;

	s3d::Vec2	m_drawPos;

	static shared_ptr<PartModel>	m_selectedPart;

public:
	PartPaletteViewer(const shared_ptr<CellModel>& model)
		: m_model(model)
	{
		m_drawRect = s3d::RectF(450, 0, 150, 450);
	}

	template <typename T>
	void	DrawModels(const string& name)
	{
		static s3d::Font font13(13);
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
			auto f = font10(s3d::Unicode::Widen((*it)->m_name));

			f.region().draw(s3d::ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
			f.draw();

			if (f.region().leftClicked()) m_selectedPart = *it;

			m_drawPos.moveBy(0.0, 15.0);
		}
		m_drawPos.moveBy(-16.0, 0.0);
	}

	void	Update() override;

	static const shared_ptr<PartModel>&	GetSelectedPart() { return m_selectedPart; }
};

