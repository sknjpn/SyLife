#include "ShapeModel.h"

void ShapeModel::FromJSON(const ptree& pt)
{
	// color
	{
		auto child = pt.get_child("color");
		auto n = 0;

		for (auto it = child.begin(); n != 3; ++it)
		{
			m_color[n] = (*it).second.get_value<int>();

			++n;
		}
	}

	// verticles
	for (auto v : pt.get_child("verticles"))
		m_verticles.emplace_back(v.second.get<double>("x"), v.second.get<double>("y"));

}
