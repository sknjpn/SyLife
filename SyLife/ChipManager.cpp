#include "ChipManager.h"

#include "ViewerManager.h"
#include "WaveManager.h"

#include "Chip.h"
#include "FieldViewer.h"

unique_ptr<ChipManager> g_chipManagerPtr;

ChipManager::ChipManager()
	: m_rect(64, 64)
	, m_length(100)
{

}

void ChipManager::init()
{
	// Chips
	{
		const Size size = m_rect.size;

		m_rect.size = size;
		m_chips.resize(size);

		for (auto p : step(size))
			m_chips[p] = MakeShared<Chip>(RectF(p * m_length, Vec2(m_length, m_length)));

		// 結びつきの作成
		for (auto p : step(size))
		{
			if (p.x != 0) m_chips[p]->m_l = m_chips[p.movedBy(-1, 0)];
			if (p.y != 0) m_chips[p]->m_u = m_chips[p.movedBy(0, -1)];
			if (p.x != size.x - 1) m_chips[p]->m_r = m_chips[p.movedBy(1, 0)];
			if (p.y != size.y - 1) m_chips[p]->m_d = m_chips[p.movedBy(0, 1)];
		}
	}


	// elements
	/*
	{
		ptree pt;
		read_json("assets/generation.json", pt);

		for (auto elements : pt.get_child("elements"))
		{
			const string name = elements.second.get<string>("name");
			const auto& model = g_assetManagerPtr->getAsset<ElementAsset>(name);
			const int size = elements.second.get<int>("size");

			for (int i = 0; i < size; ++i)
			{
				const Point point = RandomPoint(m_rect);

				m_chips[point]->m_storage.Add(model, 1.0);
			}
		}
	}
	*/

	for (auto p : step(m_chips.size()))
	{
		m_chips[p]->m_nutrition = 10;
	}
}

void ChipManager::drawChips()
{
	{
		auto c = getChip(Cursor::PosF());

		if (c != nullptr) Print << c->getNutrition();

		double sum = 0.0;
		for (auto p : step(m_rect.size))
			sum += getChip(p)->getNutrition();
		Print << sum;
	}

	// 領域の取得
	Rect rect;
	{
		auto cameraRect = g_viewerManagerPtr->getViewer<FieldViewer>()->getCamera().getCameraRect();

		rect.pos = getPoint(cameraRect.pos);
		rect.size = getPoint(cameraRect.br()) - rect.pos;
	}

	rect = rect.stretched(1);

	for (const auto p : step(rect.pos, rect.size))
	{
		if (!m_rect.contains(p)) continue;
		const auto& c = m_chips[p];

		RectF(p * m_length, Vec2(m_length, m_length))
			.draw(c->getColor());

		auto v = g_waveManagerPtr->getWaveVelocity(p * m_length);
		Line(p * m_length, p * m_length + v * 50)
			.movedBy(m_length / 2.0, m_length / 2.0)
			.drawArrow(5.0, Vec2(10, 10));
	}
}

void ChipManager::updateChips()
{
	for (const auto& c : m_chips)
	{
		const Vec2 d = g_waveManagerPtr->getWaveVelocity(c->getCenter()) * 0.005;
		const double l = 0.01;
		const double w = 1.0 + l * 2;
		const RectF rect = RectF(-l, -l, w, w).movedBy(d);
		const double value = c->m_nutrition / (w * w);

		if (c->m_l != nullptr && rect.tl().x < 0.0) c->sendTo(c->m_l, value * (-rect.tl().x) * (Min(rect.br().y, 1.0) - Max(rect.tl().y, 0.0)));
		if (c->m_u && rect.tl().y < 0.0) c->sendTo(c->m_u, value * (-rect.tl().y) * (Min(rect.br().x, 1.0) - Max(rect.tl().x, 0.0)));
		if (c->m_r && rect.br().x > 1.0) c->sendTo(c->m_r, value * (rect.br().x - 1) * (Min(rect.br().y, 1.0) - Max(rect.tl().y, 0.0)));
		if (c->m_d && rect.br().y > 1.0) c->sendTo(c->m_d, value * (rect.br().y - 1) * (Min(rect.br().x, 1.0) - Max(rect.tl().x, 0.0)));
		if (c->m_l && c->m_l->m_u && rect.tl().x < 0.0 && rect.tl().y < 0.0)
			c->sendTo(c->m_l->m_u, value * (-rect.tl().x) * (-rect.tl().y));

		if (c->m_l && c->m_l->m_d && rect.tl().x < 0.0 && rect.br().y > 1.0)
			c->sendTo(c->m_l->m_d, value * (-rect.tl().x) * (rect.br().y - 1.0));

		if (c->m_r && c->m_r->m_u && rect.br().x > 1.0 && rect.tl().y < 0.0)
			c->sendTo(c->m_r->m_u, value * (rect.br().x - 1.0) * (-rect.tl().y));

		if (c->m_r && c->m_r->m_d && rect.br().x > 1.0 && rect.br().y > 1.0)
			c->sendTo(c->m_r->m_d, value * (rect.br().x - 1.0) * (rect.br().y - 1.0));
	}
}

double ChipManager::getNutrition(const Vec2& position) const
{
	return getChip(position)->getNutrition();
}

void ChipManager::addNutrition(const Vec2& position, double nutrition)
{
	getChip(position)->addNutrition(nutrition);
}

void ChipManager::pullNutrition(const Vec2& position, double nutrition)
{
	getChip(position)->pullNutrition(nutrition);
}
