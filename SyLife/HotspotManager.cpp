#include "HotspotManager.h"

#include "Hotspot.h"

unique_ptr<HotspotManager> g_hotspotManagerPtr;

/*
const shared_ptr<HotspotState>& HotspotManager::AddHotspotState(const shared_ptr<HotspotModel>& model)
{
	const auto& m = GetHotspotStates().emplace_back(make_shared<HotspotState>());

	m->setModel(model);

	return m;
}

const shared_ptr<HotspotState>& HotspotManager::AddHotspotState(const shared_ptr<HotspotModel>& model, const Vec2& position)
{
	const auto& m = AddHotspotState(model);
	m->setPosition(position);
	m->SetVelocity(RandomVec2(Random(100.0)));

	return m;
}
*/

HotspotStateAdapter::element_type HotspotStateAdapter::GetElement(const dataset_type& dataset, size_t index, size_t dim) { return dataset[index]->getPosition().elem(dim); }
HotspotStateAdapter::element_type HotspotStateAdapter::DistanceSq(const dataset_type& dataset, size_t index, const element_type* other) { return dataset[index]->getPosition().distanceFromSq(Vec2(other[0], other[1])); }

Polygon g_hotspot;
Array<pair<Vec2, Vec2>> lines;


void HotspotManager::MakeHotspots()
{
	/*
	int numHotspots = 100;

	m_hotspots.clear();

	for (int i = 0; i < numHotspots;)
	{
		Vec2 p = RandomVec2(1.0, 1.0).movedBy(-0.5, -0.5) * m_terrainModel->m_size;

		auto f = true;
		for (const auto& hc : m_hotspots) if ((hc->m_hotspotConfig->m_position - p).length() < 100.0) f = false;

		if (GetHeight(p) > 0.1 && f)
		{
			auto& h = m_hotspots.emplace_back(make_shared<HotspotState>());

			h->m_hotspotConfig = g_assetManagerPtr->makeModel<HotspotConfig>();
			h->m_hotspotConfig->m_position = p;

			++i;
		}
	}
	*/
}

void HotspotManager::Init()
{

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

void HotspotManager::UpdateHotspotStates()
{
	/*
	for (auto& h : m_hotspots)
	{
		if (RandomBool(0.03))
		{
			g_moleculeManagerPtr->AddMoleculeState(g_assetManagerPtr->getModel<MoleculeModel>("Carbon"), h->m_hotspotConfig->getPosition());
			g_moleculeManagerPtr->AddMoleculeState(g_assetManagerPtr->getModel<MoleculeModel>("Nitrogen"), h->m_hotspotConfig->getPosition());
			g_moleculeManagerPtr->AddMoleculeState(g_assetManagerPtr->getModel<MoleculeModel>("Oxygen"), h->m_hotspotConfig->getPosition());
		}
	}
	*/
}

void HotspotManager::DrawHotspotStates()
{
	/*
	Stopwatch sw(true);
	auto e = abs(sin(sw.sF() * 6.0)) * 0.8;
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

		g_hotspot
			.drawFrame(1.0, Palette::Black)
			.draw(ColorF(Palette::Red, sin(System::FrameCount() / 100.0)));
	}
	*/
}
