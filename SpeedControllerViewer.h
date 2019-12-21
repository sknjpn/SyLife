#pragma once

#include "Viewer.h"

class SpeedControllerViewer
	: public Viewer
{
	bool	m_isHighSpeed = false;
	int		m_updateCount= 0;

public:
	void init() override;
	void update() override;

	bool isHighSpeed() const { return m_isHighSpeed; }
	void setUpdateCount(int updateCount) { m_updateCount = updateCount; }
};
