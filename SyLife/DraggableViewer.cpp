#include "stdafx.h"
#include "DraggableViewer.h"

void DraggableViewer::update()
{
	if (isMouseover() && MouseL.down())
	{
		m_isGrabbed = true;
		m_grabbedPos = Cursor::PosF();
		m_deltaPos = Vec2::Zero();
	}

	if (MouseL.up()) m_isGrabbed = false;

	if (m_isGrabbed) setViewerPosInLocal(getViewerPosInLocal() + Cursor::DeltaF());
}
