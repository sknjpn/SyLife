#include "Viewer.h"

bool Viewer::IsMouseOver() const
{
	return m_drawRect.contains(s3d::Cursor::PosRaw());
}
