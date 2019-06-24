#include "ShapeModel.h"

void ShapeModel::SetFromJSON(const ptree& pt)
{
	// color
	m_color = s3d::Color(pt.get<int>("color.r"), pt.get<int>("color.g"), pt.get<int>("color.b"));

	// polygon
	{
		s3d::Array<s3d::Vec2> verticles;
		
		for (auto v : pt.get_child("verticles"))
			verticles.emplace_back(v.second.get<double>("x"), v.second.get<double>("y"));
	
		m_polygon = s3d::Polygon(verticles);
	}

	Model::SetFromJSON(pt);
}