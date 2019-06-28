#pragma once

class Viewer;

class ViewerManager
{
public:
	vector<shared_ptr<Viewer>>	m_viewers;

public:
	ViewerManager() { m_viewers.reserve(100); }

	void	Update();

	template <typename T, typename... Args>
	shared_ptr<T>	AddViewer(Args&& ...args) { return dynamic_pointer_cast<T>(m_viewers.emplace_back(make_shared<T>(args...))); }

	template <typename T>
	shared_ptr<T>	GetViewer() const;

	template <typename T>
	bool	HasViewer() const { return GetViewer<T>() != nullptr; }

	template <typename T>
	bool	DeleteViewer() const { m_viewers.erase(remove_if(m_viewers.begin(), m_viewers.end(), [](const auto& v) { return dynamic_pointer_cast<T>(v) != nullptr; }), m_viewers.end()); }
};

extern unique_ptr<ViewerManager>	g_viewerManagerPtr;

template<typename T>
inline shared_ptr<T> ViewerManager::GetViewer() const
{
	for (const auto& v : m_viewers)
		if (dynamic_pointer_cast<T>(v) != nullptr) return dynamic_pointer_cast<T>(v);

	return nullptr;
}