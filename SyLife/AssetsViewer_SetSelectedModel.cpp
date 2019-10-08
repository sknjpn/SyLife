#include "AssetsViewer.h"

#include "ViewerManager.h"

#include "ModelEditor.h"

void AssetsViewer::setSelectedModel(const shared_ptr<Model>& model)
{
	// ModelEditorを消す
	g_viewerManagerPtr->deleteViewer<ModelEditor>();

	// 対象のViewを追加
	model->makeViewer();

	// 対象モデルの設定
	m_selectedModel = model;
}