#pragma once

#include "Storage.h"

class Chip
{
	friend class ChipManager;

	double m_nutrition;

	shared_ptr<Chip>	m_l;
	shared_ptr<Chip>	m_u;
	shared_ptr<Chip>	m_r;
	shared_ptr<Chip>	m_d;
	Vec2	m_center;
	RectF	m_rect;

public:
	Chip(const RectF& rect)
		: m_center(rect.center())
		, m_rect(rect)
		, m_nutrition(0.0)
	{}

	Vec2	getCenter() const { return m_center; }
	double	getNutrition() const { return m_nutrition; }

	void	setNutrition(double nutrition) { m_nutrition = nutrition; }
	void	addNutrition(double nutrition) { m_nutrition += nutrition; }
	void	pullNutrition(double nutrition) { m_nutrition -= nutrition; }

	Color getColor() const { return Math::Lerp(Color(0, 0), Palette::Palegreen, Min(m_nutrition, 1.0)); }
	void sendTo(const shared_ptr<Chip> chip, double rate);
};

