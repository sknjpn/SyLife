#include "CellAsset.h"
#include "CellEditor.h"

#include "ViewerManager.h"

#include "PartAsset.h"
#include "PartConfig.h"

void CellAsset::draw(double a)
{
	// parts
	for (const auto& pc : m_partConfigs)
	{
		auto t2 = Transformer2D(Mat3x2::Rotate(pc->getRotation()).translated(pc->getPosition()));

		pc->getPartAsset()->draw(a);
	}
}

void CellAsset::load(const ptree& pt)
{
	Asset::load(pt);

	// parts
	for (auto part : pt.get_child("parts"))
		m_partConfigs.emplace_back(make_shared<PartConfig>())->load(part.second);
}

void CellAsset::save(ptree& pt) const
{
	Asset::save(pt);

	// parts
	{
		ptree parts;

		for (const auto& e : m_partConfigs)
		{
			ptree part; e->save(part);
			parts.push_back(std::make_pair("", part));
		}

		pt.add_child("parts", parts);
	}

	pt.put("type", "CellAsset");
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

void CellAsset::makeViewer()
{
	//g_viewerManagerPtr->makeViewer<CellEditor>()->setPartAsset(shared_from_this());
}


shared_ptr<PartConfig>& CellAsset::addPartConfig()
{
	return m_partConfigs.emplace_back(make_shared<PartConfig>());
}

void CellAsset::updateProperties()
{
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

	m_lifespanTime = 25.0;
	m_yieldTime = 5.0;
	m_bornTime = 10.0;
}

const shared_ptr<BodyAsset>& CellAsset::getBodyAsset() const
{
	for (const auto& pc : m_partConfigs)
	{
		if (dynamic_pointer_cast<BodyAsset>(pc->getPartAsset()))
			return dynamic_pointer_cast<BodyAsset>(pc->getPartAsset());
	}

	throw Error(U"BodyAssetが存在しません。");
}
