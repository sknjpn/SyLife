#include "ReleaseViewer.h"

void ReleaseViewer::Update(bool isMouseOver)
{
	s3d::Rect(m_drawRect.size.asPoint()).draw(s3d::Color(11, 22, 33, 192));
}