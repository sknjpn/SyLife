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
		m_model->m_body->m_model->GetApproximateRect().draw(s3d::ColorF(s3d::Palette::Orange, 0.2)).drawFrame(1.0, s3d::Palette::Black);

		for (const auto& s : m_model->m_body->m_model->m_shapes)
			s3d::Polygon(s.m_verticles).draw(s3d::ColorF(s.m_color, 0.5)).drawFrame(1.0, s3d::Palette::Black);
	}

	// disk
	{
		s3d::Circle(GetDiskCenter(), GetDiskRadius()).draw(s3d::ColorF(s3d::Palette::Green, 0.5)).drawFrame(2.0, s3d::Palette::Black);
	}

	// modules
	{
		for (const auto& m : m_model->m_modules)
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Rotate(m->m_rotation).translated(m->m_position));

			m->m_model->GetApproximateRect().draw(s3d::ColorF(s3d::Palette::Orange, 0.2)).drawFrame(1.0, s3d::Palette::Black);

			for (const auto& s : m->m_model->m_shapes)
				s3d::Polygon(s.m_verticles).draw(s3d::ColorF(s.m_color, 0.5)).drawFrame(1.0, s3d::Palette::Black);
		}
	}

	// equipment
	{
		for (const auto& e : m_model->m_equipments)
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Rotate(e->m_rotation).translated(e->m_position));
			e->m_rotation += 0.01;
			e->m_model->GetApproximateRect().draw(s3d::ColorF(s3d::Palette::Orange, 0.2)).drawFrame(1.0, s3d::Palette::Black);

			for (const auto& s : e->m_model->m_shapes)
				s3d::Polygon(s.m_verticles).draw(s3d::ColorF(s.m_color, 0.5)).drawFrame(1.0, s3d::Palette::Black);
		}
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

s3d::Vec2 AssemblyViewer::GetDiskCenter() const
{
	// body
	s3d::Vec2 center = m_model->m_body->m_model->m_mass * m_model->m_body->m_model->GetApproximateRect().center();

	// equipment
	for (const auto& e : m_model->m_equipments)
		center += e->m_model->m_mass * (e->m_position + e->m_model->GetApproximateRect().center().rotated(e->m_rotation));

	// module
	for (const auto& m : m_model->m_modules)
		center += m->m_model->m_mass * (m->m_position + m->m_model->GetApproximateRect().center().rotated(m->m_rotation));

	return center / GetDiskMass();
}

double AssemblyViewer::GetDiskInertia() const
{
	auto center = GetDiskCenter();
	double inertia = 0;

	// body
	{
		double w = m_model->m_body->m_model->GetApproximateRect().w;
		double h = m_model->m_body->m_model->GetApproximateRect().h;
		double dSq = (m_model->m_body->m_model->GetApproximateRect().center() - center).lengthSq();

		inertia += m_model->m_body->m_model->m_mass * ((w * w + h * h) / 12.0 + dSq);
	}

	// equipment
	for (const auto& e : m_model->m_equipments)
	{
		double w = e->m_model->GetApproximateRect().w;
		double h = e->m_model->GetApproximateRect().h;
		double dSq = ((e->m_position + e->m_model->GetApproximateRect().center().rotated(e->m_rotation)) - center).lengthSq();

		inertia += e->m_model->m_mass * ((w * w + h * h) / 12.0 + dSq);
	}

	// module
	for (const auto& m : m_model->m_modules)
	{
		double w = m->m_model->GetApproximateRect().w;
		double h = m->m_model->GetApproximateRect().h;
		double dSq = ((m->m_position + m->m_model->GetApproximateRect().center().rotated(m->m_rotation)) - center).lengthSq();

		inertia += m->m_model->m_mass * ((w * w + h * h) / 12.0 + dSq);
	}

	return inertia;
}

double AssemblyViewer::GetDiskRadius() const
{
	return sqrt(2 * GetDiskInertia() / GetDiskMass());
}

double AssemblyViewer::GetDiskMass() const
{
	// body
	double result = m_model->m_body->m_model->m_mass;

	// equipment
	for (const auto& e : m_model->m_equipments) result += e->m_model->m_mass;

	// module
	for (const auto& m : m_model->m_modules) result += m->m_model->m_mass;

	return result;
}
