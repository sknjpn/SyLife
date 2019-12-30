#include "CellAsset.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "PartAsset_Body.h"
#include "PartAsset_Nucleus.h"

void CellAsset::draw(double a)
{
	// parts
	for (const auto& pc : m_partConfigs)
	{
		auto t2 = Transformer2D(pc->getMat3x2());

		pc->getPartAsset()->draw(a);
	}
}

void CellAsset::load(const JSONValue& json)
{
	Asset::load(json);

	// parts
	for (const auto& partConfig : json[U"parts"].arrayView())
		m_partConfigs.emplace_back(make_shared<PartConfig>())->load(partConfig);
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

void CellAsset::updateInertia()
{
	auto centroid = getCentroid();

	m_inertia = accumulate(m_partConfigs.begin(), m_partConfigs.end(), 0.0, [&centroid](double acc, const auto& pc) {
		auto id = (pc->getCentroid() - centroid).lengthSq() * pc->getPartAsset()->getMass();
		auto is = pc->getPartAsset()->getShape().getInertia(pc->getPartAsset()->getMass());
		
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

shared_ptr<PartConfig>& CellAsset::addPartConfig()
{
	return m_partConfigs.emplace_back(make_shared<PartConfig>());
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

shared_ptr<PartAsset_Body> CellAsset::getBodyAsset() const
{
	for (const auto& pc : m_partConfigs)
	{
		if (dynamic_pointer_cast<PartAsset_Body>(pc->getPartAsset()))
			return dynamic_pointer_cast<PartAsset_Body>(pc->getPartAsset());
	}

	return nullptr;
}

shared_ptr<PartAsset_Nucleus> CellAsset::getNucleusAsset() const
{
	for (const auto& pc : m_partConfigs)
	{
		if (dynamic_pointer_cast<PartAsset_Nucleus>(pc->getPartAsset()))
			return dynamic_pointer_cast<PartAsset_Nucleus>(pc->getPartAsset());
	}

	return nullptr;
}
