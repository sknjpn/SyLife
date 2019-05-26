#pragma once

#include "Recipe.h"

class RecipeManager
{
	vector<shared_ptr<Recipe>>	m_recipes;

public:
	RecipeManager();
	~RecipeManager();
};

extern unique_ptr<RecipeManager>	g_recipeManagerPtr;