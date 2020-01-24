#pragma once

#include "EasyViewer.h"

class GUIChecker :
	public EasyViewer
{
	bool	m_isGrabbed = false;
	bool	m_isEnabled = true;
	bool	m_value = false;
	Color	m_color;

	Color	getTargetColor() const;

public:
	GUIChecker(bool isEnabled = true)
		: m_isEnabled(isEnabled)
	{}

	void	init() override { m_color = getTargetColor(); }
	void	update() override;

	void	setIsEnabled(bool isEnabled);
	void	setValue(bool value) { m_value = value; }

	bool	isGrabbed() const { return m_isGrabbed; }
	bool	isEnabled() const { return m_isEnabled; }
	bool	getValue() const { return m_value; }
};

