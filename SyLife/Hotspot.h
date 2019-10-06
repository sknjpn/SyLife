#pragma once

#include "Model.h"
#include "Particle.h"
#include "ModelViewer.h"
#include "Storage.h"

class HotspotModel
	: public Model
{
	friend class HotspotViewer;

	double	m_interval;
	Storage	m_ingradient;

public:
	double	GetInterval() const { return m_interval; }
	const Storage&	GetIngradient() const { return m_ingradient; }

	void	MakeViewer();

	// JSON
	void	load_this(const ptree& pt)
	{
		// interval
		m_interval = pt.get<double>("interval");

		// ingradient
		m_ingradient.load(pt.get_child("ingradient"));

		Model::load_this(pt);
	}
	void	load(const ptree& pt) override { load_this(pt); }
	void	Save_this(ptree& pt) const
	{
		// interval
		pt.put<double>("interval", m_interval);

		// ingradient
		{
			ptree ingradient;

			m_ingradient.Save(ingradient);

			pt.add_child("ingradient", ingradient);
		}

		Model::Save_this(pt);

		pt.put("type", "HotspotModel");
	}
	void	Save(ptree& pt) const override { Save_this(pt); }
	string	GetFilepath() const override { return "assets/models/hotspots/" + GetFilename(); }
};

class HotspotConfig
	: public Model
{
public:
	shared_ptr<HotspotModel>	m_hotspotModel;
	Vec2	m_position;

public:
	// Get
	const shared_ptr<HotspotModel>&	GetHotspotModel() const { return m_hotspotModel; }
	const Vec2&	GetPosition() const { return m_position; }

	// JSON
	void	load_this(const ptree& pt)
	{
		// model
		m_hotspotModel = g_assetManagerPtr->GetModel<HotspotModel>(pt.get<string>("model"));

		// position
		m_position = Vec2(pt.get<double>("position.x"), pt.get<double>("position.y"));

		Model::load_this(pt);
	}
	void	load(const ptree& pt) override { load_this(pt); }
	void	Save_this(ptree& pt) const
	{
		// model
		pt.put("model", m_hotspotModel->GetName());

		// position
		{
			ptree position;

			position.put("x", m_position.x);
			position.put("y", m_position.y);

			pt.push_back(std::make_pair("position", position));
		}

		Model::Save_this(pt);

		pt.put("type", "PartConfig");
	}
	void	Save(ptree& pt) const override { Save_this(pt); }
};

class HotspotState
	: public Particle
{
public:
	shared_ptr<HotspotConfig>	m_hotspotConfig;

	double	m_timer = 0.0;

public:
	void	UpdateHotspot();

	void	Draw();
};

class HotspotViewer
	: public ModelViewer
{
public:

};

inline void HotspotModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<HotspotViewer>()->SetModel(shared_from_this());
}
