#pragma once

#include "Model.h"

class PartModel;

class PartConfig
	: public Model
{
	shared_ptr<PartModel>	m_partModel;
	Vec2	m_position;
	double	m_rotation;

public:
	// Get
	const shared_ptr<PartModel>& getModel() const { return m_partModel; }
	const Vec2& GetPosition() const { return m_position; }
	double	GetRotation() const { return m_rotation; }

	// Set
	void	setModel(const shared_ptr<PartModel>& partModel) { m_partModel = partModel; }
	void	SetPosition(const Vec2& position) { m_position = position; }
	void	SetRotation(double rotation) { m_rotation = rotation; }

	double	GetInertia() const { return m_partModel->GetRectInertia() + (m_position + m_partModel->GetCenter().rotated(m_rotation)).lengthSq() * m_partModel->getMass(); }

	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
};
