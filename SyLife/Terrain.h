#pragma once

#include "Model.h"
#include "ModelViewer.h"

#include "Hotspot.h"

class TerrainViewer;
class TerrainGenerator;
class HotspotConfig;

class TerrainModel
	: public Model
{
	friend class TerrainViewer;
	friend class TerrainGenerator;

	double	m_size;
	uint32	m_noiseSeed;
	double	m_density;

	// ê∂ê¨Ç≥ÇÍÇÈÇ‡ÇÃ
	Texture	m_texture;
	PerlinNoise	m_noise;
	vector<HotspotConfig>	m_hotspots;

public:

	double	GetHeight(const Vec2& position) const
	{
		auto n = abs(m_noise.octaveNoise(position / m_size * m_density, 8));
		auto m = EaseInOut(Easing::Sine, 0.0, 1.0, n);
		auto l = Min((position / m_size).length() * 2.0, 1.0);

		return EaseIn(Easing::Expo, m, 0.0, l);
	}

	Color	GetColor(const Vec2& position)
	{
		return Math::Lerp(Color(11, 22, 33), Palette::Cyan, int(GetHeight(position) * 10.0) / 10.0);
	}

	void	MakeViewer() override;

	void	MakeTexture(int textureSize = 2048)
	{
		auto image = Image(textureSize, textureSize);

		for (int x = 0; x < textureSize; ++x)
			for (int y = 0; y < textureSize; ++y)
				image[y][x] = GetColor(Vec2(x - textureSize / 2, y - textureSize / 2) * m_size / double(textureSize));

		m_texture = Texture(image);
	}

	void	MakeHotspots(int numHotspots = 300)
	{
		m_hotspots.clear();

		for (int i = 0; i < numHotspots;)
		{
			Vec2 p = RandomVec2(1.0, 1.0).movedBy(-0.5, -0.5) * m_size;

			auto f = true;
			for (const auto& hc : m_hotspots) if ((hc.m_position - p).length() < 6.0) f = false;

			if (GetHeight(p) > 0.1 && f)
			{
				auto& h = m_hotspots.emplace_back();

				h.m_position = p;

				++i;
			}
		}
	}

	void	Draw() 
	{
		m_texture.resized(m_size, m_size).drawAt(Vec2::Zero()); 

		for (const auto& hc : m_hotspots)
		{
			Circle(hc.m_position, 3).draw(ColorF(Palette::Red, 0.5));
		}
	}

	void	UpdateProperties()
	{
		m_noise = PerlinNoise(m_noiseSeed);

		MakeTexture();
		MakeHotspots();
	}

	// JSON
	void	Load_this(const ptree& pt)
	{
		// size
		m_size = pt.get<double>("size");

		// noise_seed
		m_noiseSeed = pt.get<uint32>("noise_seed");

		// density
		m_density = pt.get<double>("density");

		Model::Load_this(pt);

		UpdateProperties();
	}
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const
	{
		// size
		pt.put<double>("size", m_size);

		// noise_seed
		pt.put<double>("noise_seed", m_noiseSeed);

		// density
		pt.put<double>("density", m_density);

		Model::Save_this(pt);

		pt.put("type", "TerrainModels");
	}
	void	Save(ptree& pt) const override { Save_this(pt); }
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
