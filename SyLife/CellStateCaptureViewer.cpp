#include "FieldViewer.h"
#include "CellState.h"
#include "CellStateCaptureViewer.h"

CellStateCaptureViewer::CellStateCaptureViewer(const shared_ptr<CellState>& cellState)
	: m_cellState(cellState)
{
	
}

void CellStateCaptureViewer::update()
{
	if (MouseL.up() || m_cellState->isDestroyed())
	{
		destroy();
		return;
	}

	// CellStateを移動する
	auto fv = getParentViewer<FieldViewer>();
	auto cursorPos = fv->getCamera().getGraphics2DMat3x2().transform(Cursor::PosF());
	auto pos = m_cellState->getPosition().lerp(cursorPos, 0.1);
	m_cellState->setPosition(pos);
}
