unique_ptr<SystemManager>	g_systemManagerPtr;

SystemManager::SystemManager()
{
	g_assetManagerPtr = make_unique<AssetManager>();
	g_cellManagerPtr = make_unique<CellManager>();
	g_fieldManagerPtr = make_unique<FieldManager>();
	g_viewerManagerPtr = make_unique<ViewerManager>();
	g_eggManagerPtr = make_unique<EggManager>();
	g_waveManagerPtr = make_unique<WaveManager>();
	g_terrainManagerPtr = make_unique<TerrainManager>();

	g_assetManagerPtr->Init();
	g_fieldManagerPtr->Init();
}

void	SystemManager::Update()
{
	g_viewerManagerPtr->Update();
}
