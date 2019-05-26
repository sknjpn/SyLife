#include "RecipeManager.h"

unique_ptr<RecipeManager>	g_recipeManagerPtr;

RecipeManager::RecipeManager()
{
}


RecipeManager::~RecipeManager()
{
}

const shared_ptr<Recipe>& RecipeManager::AddRecipe()
{
	return m_recipes.emplace_back(make_shared<Recipe>());
}
