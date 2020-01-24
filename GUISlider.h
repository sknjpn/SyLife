#pragma once

#include "EasyViewer.h"

class GUISlider
	: public EasyViewer
{
	bool	m_isGrabbed = false;
	double	m_grabbedPos;
	double	m_height;
	double	m_value;
	Color	m_color;

	Color	getTargetColor() const { return m_isGrabbed ? Color(192) : getButton().mouseOver() ? Palette::Skyblue : Palette::White; }
	RectF	getButton() const { return RectF(getViewerSize().x, getButtonHeight()).setPos(0, m_value * (getViewerSize().y - getButtonHeight())); }
	double	getButtonHeight() const { return  Min(getViewerSize().y * (getViewerSize().y / m_height), getViewerSize().y); }

public:
	GUISlider(double height)
		: m_height(height)
		, m_value(0.0)
	{}

	void	init() override { m_color = getTargetColor(); }
	void	update() override;

	void	setHeight(double height) { m_height = height; }

	bool	isGrabbed() const { return m_isGrabbed; }

	void	setValue(double value) { m_value = value; }
	double	getValue() const { return m_value; }

	double	getDelta() const { return m_value * (m_height - getViewerSize().y); }
};
