#pragma once

#include "Recipe.h"

class RecipeManager
{
	vector<shared_ptr<Recipe>>	m_recipes;

public:
	const shared_ptr<Recipe>&	AddRecipe();
};

extern unique_ptr<RecipeManager>	g_recipeManagerPtr;