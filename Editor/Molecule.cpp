#include "Molecule.h"

#include "ViewerManager.h"

void MoleculeModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<MoleculeViewer>(dynamic_pointer_cast<MoleculeModel>(shared_from_this()));
}

void MoleculeModel::AddToJSON(ptree& pt) const
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

	Model::AddToJSON(pt);

	pt.put("type", "MoleculeModel");
}

void MoleculeModel::SetFromJSON(const ptree& pt)
{
	// mass
	m_mass = pt.get<double>("mass");

	// color
	m_color = s3d::Color(pt.get<int>("color.r"), pt.get<int>("color.g"), pt.get<int>("color.b"));

	// radius
	m_radius = sqrt(m_mass);

	Model::SetFromJSON(pt);
}

void MoleculeViewer::Update()
{
	// name
	{
		s3d::SimpleGUI::TextBox(m_textEditState_name, s3d::Vec2(10, 10), 240);
		m_model->m_name = s3d::Unicode::Narrow(m_textEditState_name.text);
	}

	// mass
	{
		s3d::SimpleGUI::TextBox(m_textEditState_mass, s3d::Vec2(10, 50), 240);
		m_model->m_mass = s3d::Parse<double>(m_textEditState_mass.text);
	}
}
