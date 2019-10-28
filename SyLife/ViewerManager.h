#pragma once

class Viewer;

// 表示を行うViewerの保存と更新
class ViewerManager
{
public:
	shared_ptr<Viewer>	m_rootViewer;

	void	update(const shared_ptr<Viewer>& viewer);

public:
	ViewerManager();

	void	update();

	Array<shared_ptr<Viewer>>	getViewers() const;

	const shared_ptr<Viewer>& getRootViewer() const { return m_rootViewer; }
};

extern unique_ptr<ViewerManager>	g_viewerManagerPtr;
