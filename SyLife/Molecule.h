#pragma once


#include "Model.h"

#include "Rigidbody.h"

#include "Molecule.h"


class MoleculeModel
	: public Model
{
	double		m_mass;
	double		m_radius;
	s3d::Color	m_color;

public:
	// Get
	double	GetMass() const { return m_mass; }
	double	GetRadius() const { return m_radius; }
	const s3d::Color&	GetColor() const { return m_color; }

	// JSON
	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class MoleculeState
	: public Rigidbody
{
public:
	shared_ptr<MoleculeModel>	m_model;
	
public:

};

inline void MoleculeModel::SetFromJSON(const ptree& pt)
{
	// mass
	m_mass = pt.get<double>("mass");

	// color
	m_color = s3d::Color(pt.get<int>("color.r"), pt.get<int>("color.g"), pt.get<int>("color.b"));

	// radius
	m_radius = sqrt(m_mass);

	Model::SetFromJSON(pt);
}
