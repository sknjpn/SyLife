﻿#include "FieldViewer.h"
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

	if (isMouseover()) getParentViewer<FieldViewer>()->getCamera().update();

	{
		// CellStateを移動する
		const auto t = getParentViewer<FieldViewer>()->getCamera().createTransformer();
		const auto pos = m_cellState->getPosition().lerp(Cursor::PosF(), 0.25);
		m_cellState->setPosition(pos);
	}
}