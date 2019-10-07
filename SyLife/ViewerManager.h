﻿#pragma once

class Viewer;

// 表示を行うViewerの保存と更新
class ViewerManager
{
public:
	Array<shared_ptr<Viewer>>	m_viewers;

public:
	ViewerManager() { m_viewers.reserve(1024); }

	void	update();

	template <typename T, typename... Args>
	shared_ptr<T>	makeViewer(Args&& ...args) { auto v = dynamic_pointer_cast<T>(m_viewers.emplace_back(make_shared<T>(args...))); v->Init(); return v; }

	template<typename T>
	shared_ptr<T> GetViewer() const
	{
		for (const auto& v : m_viewers)
			if (dynamic_pointer_cast<T>(v) != nullptr) return dynamic_pointer_cast<T>(v);

		return nullptr;
	}

	template <typename T>
	bool	HasViewer() const { return GetViewer<T>() != nullptr; }

	template <typename T>
	void	DeleteViewer()
	{
		// m_viewers.erase(remove_if(m_viewers.begin(), m_viewers.end(), [](const auto& v) { return dynamic_pointer_cast<T>(v) != nullptr; }), m_viewers.end()); 

		// nullptrにすることで、配列の配置を変えない
		for (auto& v : m_viewers)
			if (dynamic_pointer_cast<T>(v) != nullptr) v = nullptr;
	}

	void	ClearViewers()
	{
		// nullptrにすることで、配列の配置を変えない
		for (auto& v : m_viewers) v = nullptr;
	}
};

extern unique_ptr<ViewerManager>	g_viewerManagerPtr;
