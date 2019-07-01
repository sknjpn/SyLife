#include "AssemblyViewer.h"
#include "AssetManager.h"
#include "PartPaletteViewer.h"

#include "Body.h"
#include "Equipment.h"
#include "Module.h"

void AssemblyViewer::Init()
{
	m_camera.setCenter(m_drawRect.center());
}

void AssemblyViewer::Update()
{
	if (IsMouseOver()) m_camera.update();

	const auto t1 = m_camera.createTransformer();
	const int scale = (int)log10(m_camera.getMagnification());
	const double thickness = 2.0 / m_camera.getMagnification();
	const double interval = pow(10.0, -scale + 1);
	const auto cursor = (Cursor::Pos() / interval).asPoint() * interval;
	const auto boxSize = Vec2::One() * 5.0 / m_camera.getMagnification();
	const auto pointer = RectF(Arg::center(cursor), boxSize);

	// cü
	{
		const auto color = ColorF(Palette::White, 0.25);

		for (double x = 0; x >= m_camera.getCameraRect().x; x -= interval)
			Line(x, m_camera.getCameraRect().y, x, m_camera.getCameraRect().br().y).draw(thickness, color);

		for (double x = 0; x <= m_camera.getCameraRect().br().x; x += interval)
			Line(x, m_camera.getCameraRect().y, x, m_camera.getCameraRect().br().y).draw(thickness, color);

		for (double y = 0; y >= m_camera.getCameraRect().y; y -= interval)
			Line(m_camera.getCameraRect().x, y, m_camera.getCameraRect().br().x, y).draw(thickness, color);

		for (double y = 0; y <= m_camera.getCameraRect().br().y; y += interval)
			Line(m_camera.getCameraRect().x, y, m_camera.getCameraRect().br().x, y).draw(thickness, color);
	}

	// XYŽ²
	{
		Line(m_camera.getCameraRect().x, 0, m_camera.getCameraRect().br().x, 0).draw(thickness, Palette::Red);
		Line(0, m_camera.getCameraRect().y, 0, m_camera.getCameraRect().br().y).draw(thickness, Palette::Red);
	}

	// disk
	{
		Circle(m_radius).draw(ColorF(Palette::Green, 0.5)).drawFrame(2.0, Palette::Black);
	}

	// part
	{
		for (const auto& p : m_model->m_partConfigs)
		{
			auto t2 = Transformer2D(Mat3x2::Rotate(p->m_rotation).translated(p->m_position));

			p->m_model->GetApproximateRect().draw(ColorF(Palette::Orange, 0.2)).drawFrame(1.0, Palette::Black);

			for (const auto& s : p->m_model->m_shapes)
				Polygon(s.m_verticles).draw(ColorF(s.m_color, 0.5)).drawFrame(1.0, Palette::Black);
		}
	}

	// selectedPart
	if (PartPaletteViewer::GetSelectedPart() != nullptr)
	{
		for (const auto& s : PartPaletteViewer::GetSelectedPart()->m_shapes)
			Polygon(s.m_verticles).drawTransformed(0.0, 1.0, Cursor::Pos(), ColorF(s.m_color, 0.5));

		if (MouseL.up())
		{
			const auto& partConfig = m_model->m_partConfigs.emplace_back(make_shared<PartConfig>());

			partConfig->m_model = PartPaletteViewer::GetSelectedPart();
			partConfig->m_position = Cursor::PosF();
			partConfig->m_rotation = 0.0;
		}

		if (!MouseL.pressed()) PartPaletteViewer::m_selectedPart = nullptr;
	}

	CalculateDisk();
}

void AssemblyViewer::CalculateDisk()
{
	// mass
	{
		m_mass = 0.0;

		for (const auto& p : m_model->m_partConfigs) m_mass += p->m_model->m_mass;
	}

	// center
	{
		// body
		Vec2 center(0.0, 0.0);

		for (const auto& p : m_model->m_partConfigs) center += p->m_model->m_mass * (p->m_position + p->m_model->GetApproximateRect().center().rotated(p->m_rotation));

		center /= m_mass;

		// ˆÊ’u’²®
		for (const auto& p : m_model->m_partConfigs) p->m_position -= center;
	}

	// inertia
	{
		m_inertia = 0.0;

		for (const auto& p : m_model->m_partConfigs) m_inertia += p->GetInertia();
	}

	// radius
	m_radius = sqrt(2 * m_inertia / m_mass);
}
