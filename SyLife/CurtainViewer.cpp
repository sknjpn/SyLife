#include "CurtainViewer.h"

void CurtainViewer::update()
{
	RectF(getViewerSize()).draw(getColor());

	if (m_st.sF() > m_duration)
	{
		if (m_functionOnEnd) m_functionOnEnd();

		destroy();
	}
}
