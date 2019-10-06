#include "Molecule.h"

#include "ViewerManager.h"

void MoleculeModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<MoleculeViewer>()->SetModel(shared_from_this());
}

void MoleculeModel::load_this(const ptree& pt)
{
	// mass
	m_mass = pt.get<double>("mass");

	// color
	m_color = Color(pt.get<int>("color.r"), pt.get<int>("color.g"), pt.get<int>("color.b"));

	// radius
	m_radius = sqrt(m_mass);

	Model::load_this(pt);
}

void MoleculeModel::save_this(ptree& pt) const
{
	// mass
	pt.put<double>("mass", m_mass);

	// color
	{
		ptree color;

		color.put("r", m_color.r);
		color.put("g", m_color.g);
		color.put("b", m_color.b);

		pt.add_child("color", color);
	}

	Model::save_this(pt);

	pt.put("type", "MoleculeModel");
}
