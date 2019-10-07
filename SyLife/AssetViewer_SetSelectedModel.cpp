#include "AssetViewer.h"

#include "ViewerManager.h"

#include "ModelEditor.h"

void AssetViewer::SetSelectedModel(const shared_ptr<Model>& model)
{
	// ModelEditorを消す
	g_viewerManagerPtr->DeleteViewer<ModelEditor>();

	// 対象のViewを追加
	model->makeViewer();

	// 対象モデルの設定
	m_selectedModel = model;
}