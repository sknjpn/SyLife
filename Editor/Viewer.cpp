#include "Viewer.h"

bool Viewer::IsMouseOver() const
{
	return m_drawRect.contains(Cursor::PosRaw());
}
