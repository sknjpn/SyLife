#include "Hotspot.h"

#include "AssetManager.h"
#include "SystemManager.h"
#include "ViewerManager.h"

void HotspotState::UpdateHotspot()
{
	m_timer -= g_systemManagerPtr->GetDeltaTime();

	if (m_timer < 0.0)
	{

	}
}

void HotspotState::Draw()
{
	auto t = Transformer2D(Mat3x2::Translate(GetPosition()));

	for (int s = 100; s > 0; s -= 10)
		Circle(s).draw(ColorF(Palette::Darkred, 1.0 - s / 100.0));
}

void HotspotModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<HotspotViewer>()->SetModel(shared_from_this());
}

void HotspotConfig::load_this(const ptree& pt)
{
	// model
	m_hotspotModel = g_assetManagerPtr->GetModel<HotspotModel>(pt.get<string>("model"));

	// position
	m_position = Vec2(pt.get<double>("position.x"), pt.get<double>("position.y"));

	Model::load_this(pt);
}
