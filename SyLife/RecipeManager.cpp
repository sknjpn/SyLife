#include "RecipeManager.h"

unique_ptr<RecipeManager>	g_recipeManagerPtr;

const shared_ptr<Recipe>& RecipeManager::AddRecipe()
{
	return m_recipes.emplace_back(make_shared<Recipe>());
}
