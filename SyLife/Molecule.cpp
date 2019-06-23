#include "Molecule.h"

void MoleculeModel::SetFromJSON(const ptree & pt)
{
	// mass
	m_mass = pt.get<double>("mass");

	// color
	m_color = s3d::Color(pt.get<int>("color.r"), pt.get<int>("color.g"), pt.get<int>("color.b"));

	// radius
	m_radius = sqrt(m_mass);

	Model::SetFromJSON(pt);
}