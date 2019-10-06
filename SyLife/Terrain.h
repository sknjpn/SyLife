#pragma once

#include "Model.h"
#include "ModelViewer.h"

#include "Hotspot.h"

class TerrainViewer;
class HotspotConfig;

class TerrainModel
	: public Model
{
public:
	friend class TerrainViewer;
	friend class TerrainManager;

	double	m_size;
	uint32	m_noiseSeed;
	double	m_density;
	Array<HotspotConfig>	m_hotspots;

public:
	void	MakeViewer() override;

	// JSON
	void	load_this(const ptree& pt)
	{
		// size
		m_size = pt.get<double>("size");

		// noise_seed
		m_noiseSeed = pt.get<uint32>("noise_seed");

		// density
		m_density = pt.get<double>("density");

		Model::load_this(pt);

		// UpdateProperties();
	}
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const
	{
		// size
		pt.put<double>("size", m_size);

		// noise_seed
		pt.put<double>("noise_seed", m_noiseSeed);

		// density
		pt.put<double>("density", m_density);

		Model::save_this(pt);

		pt.put("type", "TerrainModels");
	}
	void	save(ptree& pt) const override { save_this(pt); }
	string	GetFilepath() const override { return "assets/models/hotspots/" + GetFilename(); }
};

class TerrainViewer
	: public ModelViewer
{
public:

	void	Update_this()
	{
		ModelViewer::Update_this();


	}
	void	Update() override { Update_this(); }
};

inline void TerrainModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<TerrainViewer>()->SetModel(shared_from_this());
}
