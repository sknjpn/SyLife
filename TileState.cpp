#include "TileState.h"
#include "World.h"

void TileState::update()
{
	// 平滑化
	{
		// 周囲
		const double value = m_nutrition;
		for (auto p : step(Size(3, 3)))
		{
			if (!m_nearTiles[p.x][p.y]) continue;

			m_nearTiles[p.x][p.y]->m_nutrition += value * m_sendRate[p.x][p.y];
		}

		m_nutrition = value * m_sendRate[1][1];
	}
}

void TileState::draw()
{
	getRegion().draw(getColor());

	//Line(getCentroid(), getCentroid().movedBy(getWaveVelocity() * World::GetInstance()->getTileLength())).drawArrow(20.0, Vec2(20.0, 20.0), ColorF(1.0, 0.2));
}

RectF TileState::getRegion() const
{
	return RectF(m_point * World::GetInstance()->getTileLength(), World::GetInstance()->getTileLength(), World::GetInstance()->getTileLength());
}

void TileState::setWaveVelocity(const Vec2& waveVelocity)
{
	m_waveVelocity = waveVelocity;

	// 周囲のTileの登録
	for (const auto point : step(m_point.movedBy(-1, -1), Size(3, 3)))
	{
		if (point == m_point || point.x < 0 || point.y < 0 || point.x >= World::GetInstance()->getTileSize().x || point.y >= World::GetInstance()->getTileSize().y) continue;

		m_nearTiles[point.x - m_point.x + 1][point.y - m_point.y + 1] = World::GetInstance()->getTile(point);
	}

	// SendRateの計算
	{
		const Vec2 d = getWaveVelocity() * 0.015;
		const double l = 0.01;
		const double w = 1.0 + l * 2;
		const RectF rect = RectF(-l, -l, w, w).movedBy(d);
		const double area = rect.area();

		// 初期化
		for (auto p : step(Size(3, 3)))
			m_sendRate[p.x][p.y] = 0.0;

		// 周囲
		if (rect.tl().x < 0.0) m_sendRate[0][1] = (-rect.tl().x) * (Min(rect.br().y, 1.0) - Max(rect.tl().y, 0.0)) / area;
		if (rect.tl().y < 0.0) m_sendRate[1][0] = (-rect.tl().y) * (Min(rect.br().x, 1.0) - Max(rect.tl().x, 0.0)) / area;
		if (rect.br().x > 1.0) m_sendRate[2][1] = (rect.br().x - 1) * (Min(rect.br().y, 1.0) - Max(rect.tl().y, 0.0)) / area;
		if (rect.br().y > 1.0) m_sendRate[1][2] = (rect.br().y - 1) * (Min(rect.br().x, 1.0) - Max(rect.tl().x, 0.0)) / area;
		if (rect.tl().x < 0.0 && rect.tl().y < 0.0) m_sendRate[0][0] = (-rect.tl().x) * (-rect.tl().y) / area;
		if (rect.tl().x < 0.0 && rect.br().y > 1.0) m_sendRate[0][2] = (-rect.tl().x) * (rect.br().y - 1.0) / area;
		if (rect.br().x > 1.0 && rect.tl().y < 0.0) m_sendRate[2][0] = (rect.br().x - 1.0) * (-rect.tl().y) / area;
		if (rect.br().x > 1.0 && rect.br().y > 1.0) m_sendRate[2][2] = (rect.br().x - 1.0) * (rect.br().y - 1.0) / area;

		// 中心
		m_sendRate[1][1] = 1.0 - m_sendRate[0][0] - m_sendRate[1][0] - m_sendRate[2][0] - m_sendRate[0][1] - m_sendRate[2][1] - m_sendRate[0][2] - m_sendRate[1][2] - m_sendRate[2][2];

		// 存在しないところの分を移動
		if (!m_nearTiles[0][1])
		{
			for (int i = 0; i < 3; ++i)
			{
				m_sendRate[1][i] += m_sendRate[0][i];
				m_sendRate[0][i] = 0;
			}
		}
		if (!m_nearTiles[1][0])
		{
			for (int i = 0; i < 3; ++i)
			{
				m_sendRate[i][1] += m_sendRate[i][0];
				m_sendRate[i][0] = 0;
			}
		}
		if (!m_nearTiles[2][1])
		{
			for (int i = 0; i < 3; ++i)
			{
				m_sendRate[1][i] += m_sendRate[2][i];
				m_sendRate[2][i] = 0;
			}
		}
		if (!m_nearTiles[1][2])
		{
			for (int i = 0; i < 3; ++i)
			{
				m_sendRate[i][1] += m_sendRate[i][2];
				m_sendRate[i][2] = 0;
			}
		}
	}
}

Vec2 TileState::getCentroid() const
{
	return Vec2(m_point * World::GetInstance()->getTileLength()).movedBy(World::GetInstance()->getTileLength() / 2.0, World::GetInstance()->getTileLength() / 2.0);
}

void TileState::sendTo(const shared_ptr<TileState>& tile, double value)
{
	tile->m_nutrition += value;
	m_nutrition -= value;
}

void TileState::load(Deserializer<ByteArray>& reader)
{
	{
		Vec2 waveVelocity;
		reader >> waveVelocity;
		setWaveVelocity(waveVelocity);
	}

	reader >> m_nutrition;
}

void TileState::save(Serializer<MemoryWriter>& writer) const
{
	writer << m_waveVelocity;
	writer << m_nutrition;
}
