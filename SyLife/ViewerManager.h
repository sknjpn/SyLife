#pragma once

class Viewer;

// 表示を行うViewerの保存と更新
class ViewerManager
{
public:
	Array<Viewer*>	m_viewers;

public:
	ViewerManager() { m_viewers.reserve(1024); }

	void	update();

	void	registerViewer(Viewer* viewer) { m_viewers.emplace_back(viewer); }
	void	deregisterViewer(Viewer* viewer) { m_viewers.remove(viewer); }

	template<typename T>
	T* getViewer() const
	{
		for (const auto& v : m_viewers)
			if (dynamic_cast<T*>(v) != nullptr) return dynamic_cast<T*>(v);

		return nullptr;
	}

	template <typename T>
	bool	hasViewer() const { return getViewer<T>() != nullptr; }
};

extern unique_ptr<ViewerManager>	g_viewerManagerPtr;
