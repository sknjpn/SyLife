#include "CellAsset.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "PartAsset_Body.h"
#include "PartAsset_Nucleus.h"

void CellAsset::draw(double a)
{
	// parts
	for (const auto& partConfig : m_partConfigs)
	{
		auto t2 = Transformer2D(partConfig->getMat3x2());

		partConfig->getPartAsset()->draw(a);
	}
}

void CellAsset::load(const JSONValue& json)
{
	Asset::load(json);

	// parts
	for (const auto& partConfig : json[U"parts"].arrayView())
		m_partConfigs.emplace_back(MakeShared<PartConfig>())->load(partConfig);
}

void CellAsset::save(JSONWriter& json) const
{
	Asset::save(json);

	// parts
	json.key(U"parts").startArray();
	{
		for (const auto& partConfig : m_partConfigs)
		{
			json.startObject();

			partConfig->save(json);

			json.endObject();
		}
	}
	json.endArray();
}

Vec2 CellAsset::getCentroid()
{
	return accumulate(m_partConfigs.begin(), m_partConfigs.end(), Vec2::Zero(), [](Vec2 acc, const auto& p) { return acc + p->getPartAsset()->getMass() * (p->getPosition() + p->getPartAsset()->getShape().getCentroid().rotated(p->getRotation())); }) / m_mass;
}

void CellAsset::updateMass()
{
	m_mass = accumulate(m_partConfigs.begin(), m_partConfigs.end(), 0.0, [](double mass, const auto& p) { return mass + p->getPartAsset()->getMass(); });
}

void CellAsset::updateDrawRadius()
{
	m_drawRadius = 0.0;

	for (const auto& partConfig : m_partConfigs)
	{
		for (const auto& layer : partConfig->getPartAsset()->getShape())
		{
			auto polygon = layer.m_polygon.rotated(partConfig->getRotation()).movedBy(partConfig->getPosition());

			for (auto p : polygon.outer())
				m_drawRadius = Max(m_drawRadius, p.length());
		}
	}
}

void CellAsset::updateInertia()
{
	auto centroid = getCentroid();

	m_inertia = accumulate(m_partConfigs.begin(), m_partConfigs.end(), 0.0, [&centroid](double acc, const auto& partConfig) {
		auto id = (partConfig->getCentroid() - centroid).lengthSq() * partConfig->getPartAsset()->getMass();
		auto is = partConfig->getPartAsset()->getShape().getInertia(partConfig->getPartAsset()->getMass());

		return acc + id + is;
		});
}

void CellAsset::updateMaxStorage()
{
	// ストック出来る量は必要資源の２倍
	m_maxStorage = m_material + m_material;
}

void CellAsset::updateMaterial()
{
	m_material = accumulate(m_partConfigs.begin(), m_partConfigs.end(), Storage(), [](Storage acc, const auto& p) { return acc += p->getPartAsset()->getMaterial(); });
}

RectF CellAsset::getCellAssetDrawRegion() const
{
	Array<RectF> regions;

	for (const auto& partConfig : m_partConfigs)
		if (partConfig->getPartAsset()->drawOnAssetEnabled())
			regions.emplace_back(partConfig->getPartAsset()->getShape().getPolygon().rotated(partConfig->getRotation()).movedBy(partConfig->getPosition()).boundingRect());

	RectF maxRegion = regions.front();
	for (const auto& region : regions)
	{
		maxRegion.x = Min(maxRegion.x, region.x);
		maxRegion.y = Min(maxRegion.y, region.y);
	}
	
	for (const auto& region : regions)
	{
		maxRegion.w = Max(maxRegion.tr().x, region.tr().x) - maxRegion.x;
		maxRegion.h = Max(maxRegion.tr().y, region.tr().y) - maxRegion.y;
	}

	return maxRegion;
}

RectF CellAsset::getCellStateDrawRegion() const
{
	Array<RectF> regions;

	for (const auto& partConfig : m_partConfigs)
		if (partConfig->getPartAsset()->drawOnAssetEnabled())
			regions.emplace_back(partConfig->getPartAsset()->getShape().getPolygon().rotated(partConfig->getRotation()).movedBy(partConfig->getPosition()).boundingRect());

	RectF maxRegion = regions.front();
	for (const auto& region : regions)
	{
		maxRegion.x = Min(maxRegion.x, region.x);
		maxRegion.y = Min(maxRegion.y, region.y);
	}

	for (const auto& region : regions)
	{
		maxRegion.w = Max(maxRegion.br().x, region.br().x) - maxRegion.x;
		maxRegion.h = Max(maxRegion.br().y, region.br().y) - maxRegion.y;
	}

	return maxRegion;
}

void CellAsset::preRender()
{
	auto assetDrawRegion = getCellAssetDrawRegion();
	auto stateDrawRegion = getCellStateDrawRegion();

	assetDrawRegion = RectF(Max(-assetDrawRegion.br().x, assetDrawRegion.tl().x) * 2.0, Max(-assetDrawRegion.br().y, assetDrawRegion.tl().y) * 2.0).setCenter(Vec2::Zero());
	stateDrawRegion = RectF(Max(-stateDrawRegion.br().x, stateDrawRegion.tl().x) * 2.0, Max(-stateDrawRegion.br().y, stateDrawRegion.tl().y) * 2.0).setCenter(Vec2::Zero());
	
	Image assetDrawImage(assetDrawRegion.size.asPoint());
	Image stateDrawImage(stateDrawRegion.size.asPoint());

	for (const auto& partConfig : m_partConfigs)
	{
		partConfig->getPartAsset()->preRender(assetDrawImage, partConfig);
		partConfig->getPartAsset()->preRender(stateDrawImage, partConfig);
	}

	m_cellAssetTexture = Texture(assetDrawImage);
	m_cellStateTexture = Texture(stateDrawImage);
}

void CellAsset::setCentroidAsOrigin()
{
	auto centroid = getCentroid();

	for (const auto& p : m_partConfigs)
		p->setPosition(p->getPosition() - centroid);
}

bool CellAsset::isValid() const
{
	return getBodyAsset() && getNucleusAsset();
}

std::shared_ptr<PartConfig>& CellAsset::addPartConfig()
{
	return m_partConfigs.emplace_back(MakeShared<PartConfig>());
}

void CellAsset::updateProperties()
{
	if (!isValid()) return;

	// mass
	updateMass();

	// inertia
	updateInertia();

	// radius
	updateRadius();

	updateDrawRadius();

	// material
	updateMaterial();

	// maxStorage (生成の必要量の二倍)
	updateMaxStorage();

	const auto nucleusAsset = getNucleusAsset();
	m_lifespanTime = nucleusAsset->getLifespanTime();
	m_yieldTime = nucleusAsset->getYieldTime();
	m_bornTime = nucleusAsset->getBornTime();

	m_maxHitPoint = getBodyAsset()->getShape().getPolygon().area();
}

std::shared_ptr<PartAsset_Body> CellAsset::getBodyAsset() const
{
	for (const auto& partConfig : m_partConfigs)
	{
		if (std::dynamic_pointer_cast<PartAsset_Body>(partConfig->getPartAsset()))
			return std::dynamic_pointer_cast<PartAsset_Body>(partConfig->getPartAsset());
	}

	return nullptr;
}

std::shared_ptr<PartAsset_Nucleus> CellAsset::getNucleusAsset() const
{
	for (const auto& partConfig : m_partConfigs)
	{
		if (std::dynamic_pointer_cast<PartAsset_Nucleus>(partConfig->getPartAsset()))
			return std::dynamic_pointer_cast<PartAsset_Nucleus>(partConfig->getPartAsset());
	}

	return nullptr;
}
