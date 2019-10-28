#pragma once

class Viewer;

// 表示を行うViewerの保存と更新
class ViewerManager
{
	shared_ptr<Viewer>	m_rootViewer;

public:
	ViewerManager();

	void	update();

	const shared_ptr<Viewer>& getRootViewer() const { return m_rootViewer; }
};

extern unique_ptr<ViewerManager>	g_viewerManagerPtr;
