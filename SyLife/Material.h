#pragma once

#include "Model.h"
#include "Storage.h"

// あるものを生産するのに必要な量
class Material
	: public Model
{
	Storage m_storage;
	double	m_nutrition;

public:
	const Storage& getStorage() const { return m_storage; }
	double	getNutrition() const { return m_nutrition; }
};

