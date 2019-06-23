#pragma once

#include "Equipment.h"

class WingModel
	: public EquipmentModel
{
public:

};

class WingConfig
	: public EquipmentConfig
{
public:

};

class WingState
	: public EquipmentState
{
	int		m_timer = 0;

public:
	void	Update() override 
	{
		if ((m_timer++) == 60)
		{
			m_timer = 0;

			Flap();
		}
	}

	void	Flap();
};

