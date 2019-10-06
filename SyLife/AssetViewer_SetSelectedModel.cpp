#include "AssetViewer.h"

#include "ViewerManager.h"

#include "ModelViewer.h"

void AssetViewer::SetSelectedModel(const shared_ptr<Model>& model)
{
	// ModelViewerを消す
	g_viewerManagerPtr->DeleteViewer<ModelViewer>();

	// 対象のViewを追加
	model->MakeViewer();

	// 対象モデルの設定
	m_selectedModel = model;
}