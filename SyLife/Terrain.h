#pragma once

#include "Model.h"
#include "ModelViewer.h"

class TerrainViewer;
class TerrainGenerator;

class TerrainModel
	: public Model
{
public:
	friend class TerrainViewer;
	friend class TerrainGenerator;

	Texture	m_texture;
	RectF	m_rect;

public:
	void	MakeViewer() override;
};

class TerrainGenerator
{
	PerlinNoise	m_noise;

public:
	TerrainGenerator()
		: m_noise(Random(0xFFFFFFFF))
	{}

	shared_ptr<TerrainModel>	MakeTerrainModel(double length, int size)
	{
		auto model = make_shared<TerrainModel>();
		auto image = Image(size, size);

		for (int x = 0; x < size; ++x)
			for (int y = 0; y < size; ++y)
				image[y][x] = GetColor(Vec2(x / double(size), y / double(size)));

		model->m_texture = Texture(image);
		model->m_rect.w = length;
		model->m_rect.h = length;

		return model;
	}

	Color	GetColor(const Vec2& p)
	{
		auto h = GetHeight(p);
		int s = int(h * 10.0);

		//return Math::Lerp(Palette::Cyan, Color(11, 22, 33), s / 10.0);

		if (s == 0) return Palette::Darkblue;
		if (s == 1) return Palette::Blue;
		if (s == 2) return Palette::Khaki;
		if (s == 3) return Palette::Lightgreen;
		if (s == 4) return Palette::Yellowgreen;
		if (s == 5) return Palette::Green;
		if (s == 6) return Palette::Darkgreen;
		if (s == 7) return Palette::Gray;
		if (s == 8) return Palette::Gray;
		if (s == 9) return Palette::White;
	}

	double	GetHeight(const Vec2& p)
	{
		auto n = abs(m_noise.octaveNoise(Vec2(p.x, p.y) * 3.0, 8));
		auto m = EaseInOut(Easing::Sine, 0.0, 1.0, n);
		auto l = Min((p - Vec2(0.5, 0.5)).length() * 2.0, 1.0);

		return Min(EaseIn(Easing::Expo, m, 0.0, l), 1.0);
	}
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
