#pragma once

#include "Viewer.h"

class ViewerManager
{
public:
	vector<shared_ptr<Viewer>>	m_viewers;

public:
	ViewerManager() { m_viewers.reserve(100); }

	void	Update();

	template <typename T, typename... Args>
	shared_ptr<T>	AddViewer(Args&& ...args)
	{
		return dynamic_pointer_cast<T>(m_viewers.emplace_back(make_shared<T>(args...)));
	}
};

extern unique_ptr<ViewerManager>	g_viewerManagerPtr;
