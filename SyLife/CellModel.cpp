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

		pc->getModel()->draw(a);
	}
}

void CellAsset::load_this(const ptree& pt)
{
	// parts
	for (auto part : pt.get_child("parts")) m_partConfigs.emplace_back(make_shared<PartConfig>())->load(part.second);

	UpdateProperties();

	Model::load_this(pt);
}

void CellAsset::save_this(ptree& pt) const
{
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

	Model::save_this(pt);

	pt.put("type", "CellAsset");
}

void CellAsset::makeViewer()
{
	g_viewerManagerPtr->makeViewer<CellEditor>()->setModel(shared_from_this());
}


shared_ptr<PartConfig>& CellAsset::AddPartConfig()
{
	return m_partConfigs.emplace_back(make_shared<PartConfig>());
}

void CellAsset::UpdateProperties()
{
	// mass
	m_mass = accumulate(m_partConfigs.begin(), m_partConfigs.end(), 0.0, [](double mass, const auto& p) { return mass + p->getModel()->getMass(); });

	// center
	Vec2 center = accumulate(m_partConfigs.begin(), m_partConfigs.end(), Vec2::Zero(), [](Vec2 acc, const auto& p) { return acc + p->getModel()->getMass() * (p->getPosition() + p->getModel()->getCenter().rotated(p->getRotation())); }) / m_mass;

	// centerを原点に設定
	for (const auto& p : m_partConfigs) p->setPosition(p->getPosition() - center);

	// inertia
	m_inertia = accumulate(m_partConfigs.begin(), m_partConfigs.end(), 0.0, [](double acc, const auto& p) { return acc + p->getInertia(); });

	// radius
	m_radius = sqrt(2 * m_inertia / m_mass);

	// material
	m_material = accumulate(m_partConfigs.begin(), m_partConfigs.end(), Storage(), [](Storage acc, const auto& p) { return acc += p->getModel()->getMaterial(); });
}