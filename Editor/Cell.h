#pragma once

#include "Model.h"
#include "Storage.h"
#include "Viewer.h"
#include "Part.h"

class CellViewer;

class CellModel
	: public Model
{
public:
	Storage	m_material;
	vector<shared_ptr<PartConfig>>	m_partConfigs;

	// ‹ßŽ—‰~
	double	m_mass;
	double	m_radius;
	double	m_inertia;

public:
	void	MakeViewers() override;

	template <typename T>
	shared_ptr<T>			GetPart(const string& name) const
	{
		for (auto it = m_partConfigs.begin(); it != m_partConfigs.end(); ++it)
			if ((*it)->m_name == name && dynamic_pointer_cast<T>(*it) != nullptr) return dynamic_pointer_cast<T>(*it);

		return nullptr;
	}

	template <typename T>
	vector<shared_ptr<T>>	GetParts() const
	{
		vector<shared_ptr<T>>	tModels;

		for (auto it = m_partConfigs.begin(); it != m_partConfigs.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) tModels.emplace_back(dynamic_pointer_cast<T>(*it));

		return tModels;
	}

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const;
	void	Save(ptree& pt) const override { AddToJSON(pt); }

	string	GetFilepath() const override { return "assets/models/cells/" + GetFilename(); }
};

class CellViewer
	: public Viewer
{
	shared_ptr<CellModel>	m_model;
	s3d::TextEditState		m_textEditState_name;

public:
	CellViewer(const shared_ptr<CellModel>& model)
		: m_model(model)
		, m_textEditState_name(s3d::Unicode::Widen(model->GetName()))
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}

	void	Update() override;
};

