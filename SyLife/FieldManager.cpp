#include "FieldManager.h"
#include "MoleculeManager.h"
#include "CellManager.h"
#include "RecipeManager.h"
#include "Rigidbody.h"

unique_ptr<FieldManager>	g_fieldManagerPtr;

FieldManager::FieldManager()
	: m_size(800, 600)
{

}

FieldManager::~FieldManager()
{

}

void FieldManager::Init()
{
	g_moleculeManagerPtr = make_unique<MoleculeManager>();
	g_cellManagerPtr = make_unique<CellManager>();
	g_recipeManagerPtr = make_unique<RecipeManager>();

	g_moleculeManagerPtr->Init();
}