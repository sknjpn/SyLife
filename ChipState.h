﻿#pragma once

#include "State.h"
#include "Storage.h"

class ChipState
	: public State
{
	double	m_nutrition;
	Point	m_point;
	Vec2	m_waveVelocity;

	double	m_sendRate[3][3];	// 周囲のマスに送る比率
	shared_ptr<ChipState>	m_nearChips[3][3];

public:
	ChipState(const Point& point)
		: m_point(point)
		, m_nutrition(0.0)
	{}

	void	update();
	void	draw();

	RectF	getRegion() const;

	const Point& getPoint() const { return m_point; }

	const Vec2& getWaveVelocity() const { return m_waveVelocity; }
	void	setWaveVelocity(const Vec2& waveVelocity);

	Vec2	getCentroid() const;
	double	getNutrition() const { return m_nutrition; }

	void	setNutrition(double nutrition) { m_nutrition = nutrition; }
	void	addNutrition(double nutrition) { m_nutrition += nutrition; }
	void	pullNutrition(double nutrition) { m_nutrition -= nutrition; }

	Color	getColor() const { return Math::Lerp(Color(0, 0), Palette::Palegreen, Min(m_nutrition / 20.0, 1.0)); }
	void	sendTo(const shared_ptr<ChipState>& chip, double value);

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
};

