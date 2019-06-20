#include "AssemblyViewer.h"
#include "AssetManager.h"
#include "PartPaletteViewer.h"

void AssemblyViewer::Init()
{
	m_camera.setCenter(m_drawRect.center());
}

void AssemblyViewer::Update()
{
	if (IsMouseOver()) m_camera.update();

	const auto t = m_camera.createTransformer();
	const int scale = log10(m_camera.getMagnification());
	const double thickness = 2.0 / m_camera.getMagnification();
	const double interval = pow(10.0, -scale + 1);
	const auto cursor = (s3d::Cursor::Pos() / interval).asPoint() * interval;
	const auto boxSize = s3d::Vec2::One() * 5.0 / m_camera.getMagnification();
	const auto pointer = s3d::RectF(s3d::Arg::center(cursor), boxSize);

	// cü
	{
		const auto color = s3d::ColorF(s3d::Palette::White, 0.25);

		for (double x = 0; x >= m_camera.getCameraRect().x; x -= interval)
			s3d::Line(x, m_camera.getCameraRect().y, x, m_camera.getCameraRect().br().y).draw(thickness, color);

		for (double x = 0; x <= m_camera.getCameraRect().br().x; x += interval)
			s3d::Line(x, m_camera.getCameraRect().y, x, m_camera.getCameraRect().br().y).draw(thickness, color);

		for (double y = 0; y >= m_camera.getCameraRect().y; y -= interval)
			s3d::Line(m_camera.getCameraRect().x, y, m_camera.getCameraRect().br().x, y).draw(thickness, color);

		for (double y = 0; y <= m_camera.getCameraRect().br().y; y += interval)
			s3d::Line(m_camera.getCameraRect().x, y, m_camera.getCameraRect().br().x, y).draw(thickness, color);
	}

	// XYŽ²
	{
		s3d::Line(m_camera.getCameraRect().x, 0, m_camera.getCameraRect().br().x, 0).draw(thickness, s3d::Palette::Red);
		s3d::Line(0, m_camera.getCameraRect().y, 0, m_camera.getCameraRect().br().y).draw(thickness, s3d::Palette::Red);
	}

	// body
	if (m_model->m_body->m_model != nullptr)
	{
		for (const auto& s : m_model->m_body->m_model->m_shapes)
		{
			s3d::Polygon(s.m_verticles).draw(s3d::ColorF(s.m_color, 0.5));
		}
	}

	// modules
	{
		for (const auto& m : m_model->m_modules)
			for (const auto& s : m->m_model->m_shapes)
				s3d::Polygon(s.m_verticles).drawTransformed(sin(m->m_rotation), cos(m->m_rotation), m->m_position, s3d::ColorF(s.m_color, 0.5));
	}

	// equipment
	{
		for (const auto& e : m_model->m_equipments)
			for (const auto& s : e->m_model->m_shapes)
				s3d::Polygon(s.m_verticles).drawTransformed(sin(e->m_rotation), cos(e->m_rotation), e->m_position, s3d::ColorF(s.m_color, 0.5));
	}

	// selectedPart
	if (PartPaletteViewer::GetSelectedPart() != nullptr)
	{
		for (const auto& s : PartPaletteViewer::GetSelectedPart()->m_shapes)
			s3d::Polygon(s.m_verticles).drawTransformed(0.0, 1.0, s3d::Cursor::Pos(), s3d::ColorF(s.m_color, 0.5));

		if (s3d::MouseL.up())
		{
			if (dynamic_pointer_cast<ModuleModel>(PartPaletteViewer::GetSelectedPart()))
			{
				auto model = dynamic_pointer_cast<ModuleModel>(PartPaletteViewer::GetSelectedPart());

				m_model->m_modules.emplace_back(make_shared<ModuleConfig>(model, s3d::Cursor::PosF(), 0.0));
			}
			if (dynamic_pointer_cast<EquipmentModel>(PartPaletteViewer::GetSelectedPart()))
			{
				auto model = dynamic_pointer_cast<EquipmentModel>(PartPaletteViewer::GetSelectedPart());

				m_model->m_equipments.emplace_back(make_shared<EquipmentConfig>(model, s3d::Cursor::PosF(), 0.0));
			}
		}

		if (!s3d::MouseL.pressed()) PartPaletteViewer::m_selectedPart = nullptr;
	}
}
