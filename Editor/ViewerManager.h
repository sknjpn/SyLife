#pragma once

#include "Viewer.h"

class ViewerManager
{
public:
	vector<shared_ptr<Viewer>>	m_viewers;

public:
	ViewerManager() { m_viewers.reserve(100); }

	void	Update();

	template <typename T>
	const shared_ptr<T>&	AddViewer(const s3d::RectF& drawRect)
	{
		const auto& v = make_shared<T>();
		
		m_viewers.emplace_back(v);

		v->m_drawRect = drawRect;

		return v;
	}
};

extern unique_ptr<ViewerManager>	g_viewerManagerPtr;
