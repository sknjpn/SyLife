#pragma once

class RecipeManager
{
public:
	RecipeManager();
	~RecipeManager();
};

extern unique_ptr<RecipeManager>	g_recipeManagerPtr;