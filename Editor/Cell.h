#pragma once

#include "Model.h"
#include "Viewer.h"
#include "Part.h"

class CellViewer;

class CellModel
	: public Model
{
public:
	vector<shared_ptr<PartConfig>>	m_parts;

public:
	void	MakeViewers() override;

	template <typename T>
	shared_ptr<T>			GetPart(const string& name) const
	{
		for (auto it = m_parts.begin(); it != m_parts.end(); ++it)
			if ((*it)->m_name == name && dynamic_pointer_cast<T>(*it) != nullptr) return dynamic_pointer_cast<T>(*it);

		return nullptr;
	}

	template <typename T>
	vector<shared_ptr<T>>	GetParts() const
	{
		vector<shared_ptr<T>>	tModels;

		for (auto it = m_parts.begin(); it != m_parts.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) tModels.emplace_back(dynamic_pointer_cast<T>(*it));

		return tModels;
	}

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	AddToJSON(ptree pt) const;
	ptree	Save() const override { return AddToJSON(ptree()); }

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
		, m_textEditState_name(s3d::Unicode::Widen(model->m_name))\
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}

	void	Update() override;
};

