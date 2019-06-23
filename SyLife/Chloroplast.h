#pragma once

#include "Module.h"

class ChloroplastModel
	: public ModuleModel
{
public:
};

class ChloroplastConfig
	: public ModuleConfig
{
public:
};

class ChloroplastState
	: public ModuleState
{
public:
	void	MakeNutrition();
};
