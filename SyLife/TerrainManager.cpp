#include "TerrainManager.h"
#include "Terrain.h"
#include "Hotspot.h"

unique_ptr<TerrainManager>	g_terrainManagerPtr;

double TerrainManager::GetHeight(const Vec2 & position) const
{
	auto n = abs(m_noise.octaveNoise(position / m_terrainModel->m_size * m_terrainModel->m_density, 6));
	auto m = EaseInOut(Easing::Sine, 0.0, 1.0, n);
	auto l = Min((position / m_terrainModel->m_size).length() * 2.0, 1.0);

	return EaseIn(Easing::Expo, m, 0.0, l);
}

Color TerrainManager::GetColor(const Vec2& position)
{
	return Color(int(GetHeight(position) * 30), 0, 0);

	//return Math::Lerp(Color(11, 22, 33), Palette::Cyan, int(GetHeight(position) * 10.0) / 10.0);
}

void TerrainManager::MakeTexture(int textureSize)
{
	auto image = Image(textureSize, textureSize);

	for (int x = 0; x < textureSize - 1; x += 2)
	{
		for (int y = 0; y < textureSize - 1; y += 2)
		{
			auto color = GetColor(Vec2(x - textureSize / 2, y - textureSize / 2) * m_terrainModel->m_size / double(textureSize));

			image[y][x] = color;
			image[y][x + 1] = color;
			image[y + 1][x] = color;
			image[y + 1][x + 1] = color;
		}
	}

	for (int i = 20; i >= 0; --i)
	{
		for (auto p : step(image.size()))
			image[p.y][p.x] = image[p.y][p.x].r > i ? Color(255) : image[p.y][p.x];

		m_polygons.emplace_back(image.grayscaleToPolygons());
	}

	m_polygons.reverse();

	for (auto& p : m_polygons)
	{
		p = p.simplified(2.0);
		p.scale(m_terrainModel->m_size / double(textureSize));
		p.moveBy(-Vec2::One() * m_terrainModel->m_size / 2.0);
	}
	//m_texture = Texture(image);
}


void TerrainManager::MakeHotspots(int numHotspots)
{
	m_hotspots.clear();

	for (int i = 0; i < numHotspots;)
	{
		Vec2 p = RandomVec2(1.0, 1.0).movedBy(-0.5, -0.5) * m_terrainModel->m_size;

		auto f = true;
		for (const auto& hc : m_hotspots) if ((hc->m_hotspotConfig->m_position - p).length() < 100.0) f = false;

		if (GetHeight(p) > 0.1 && f)
		{
			auto& h = m_hotspots.emplace_back(make_shared<HotspotState>());

			h->m_hotspotConfig = g_assetManagerPtr->MakeModel<HotspotConfig>();
			h->m_hotspotConfig->m_position = p;

			++i;
		}
	}
}

Polygon g_hotspot;
Array<pair<Vec2, Vec2>> lines;

void TerrainManager::SetTerrainModel(const shared_ptr<TerrainModel>& model)
{
	m_terrainModel = model;
	m_noise = PerlinNoise(m_terrainModel->m_noiseSeed);

	MakeTexture(0x1000);
	MakeHotspots(100);

	lines.push_back({ Vec2::Zero(), Vec2::Right().rotated(Math::TwoPi * 0.0) });
	lines.push_back({ Vec2::Zero(), Vec2::Right().rotated(Math::TwoPi * 0.2) });
	lines.push_back({ Vec2::Zero(), Vec2::Right().rotated(Math::TwoPi * 0.4) });
	lines.push_back({ Vec2::Zero(), Vec2::Right().rotated(Math::TwoPi * 0.6) });
	lines.push_back({ Vec2::Zero(), Vec2::Right().rotated(Math::TwoPi * 0.8) });

	g_hotspot.append(Circle(3).asPolygon());
	for (int i = 0; i < Min(int(lines.size()), 50); ++i)
	{
		double b = Math::Lerp(3.0, 0.1, i / 50.0);
		auto f = lines[i].first;
		auto s = lines[i].second;

		if (RandomBool(1.00)) lines.emplace_back(f + s * 15, s.rotated(Random(-1.0, 1.0)));
		if (RandomBool(0.50)) lines.emplace_back(f + s * 15, s.rotated(Random(-1.0, 1.0)));

		g_hotspot.append(Triangle(f - s * b - s.rotated(-Math::HalfPi) * b, f - s * b - s.rotated(Math::HalfPi) * b, f + s * 15.0).asPolygon());
	}
}

void TerrainManager::Update()
{

}

void TerrainManager::Draw()
{
	//m_texture.resized(m_terrainModel->m_size, m_terrainModel->m_size).drawAt(Vec2::Zero());

	for (int i = 0; i <= 20; ++i)
		m_polygons[i].draw(Math::Lerp(Color(11, 22, 33), Palette::Cyan, i / 20.0));

	auto e = abs(sin(System::FrameCount() / 10.0)) * 0.8;
	for (const auto& hc : m_hotspots)
	{
		auto t = Transformer2D(Mat3x2::Translate(hc->m_hotspotConfig->m_position));

		for (int i = 0; i < Min(int(lines.size()), 50); ++i)
		{
			double b = Math::Lerp(3.0, 0.1, i / 50.0);
			auto f = lines[i].first;
			auto s = lines[i].second;
			auto a = (1.0 - i / 50.0) * e;

			Triangle(f - s * b - s.rotated(-Math::HalfPi) * b, f - s * b - s.rotated(Math::HalfPi) * b, f + s * 15.0)
				//.drawFrame(1.0, ColorF(Palette::Black, (1.0 - i / 50.0)))
				.draw(ColorF(Palette::Red, a));
		}

		Circle(10)
			.drawFrame(2.0, ColorF(Palette::Black, 1.0))
			.draw(ColorF(Palette::Red, e));

		/*
		g_hotspot
			.drawFrame(1.0, Palette::Black)
			.draw(ColorF(Palette::Red, sin(System::FrameCount() / 100.0)));
			*/
	}
}
