
void Main()
{
	s3d::Graphics::SetBackground(s3d::ColorF(0.8, 0.9, 1.0));

	const s3d::Font font(60);

	const s3d::Texture textureCat(s3d::Emoji(U"🐈"), s3d::TextureDesc::Mipped);

	while (s3d::System::Update())
	{
		font(U"Hello, Siv3D!🐣").drawAt(s3d::Window::Center(), s3d::Palette::Black);

		font(s3d::Cursor::Pos()).draw(20, 500, s3d::ColorF(0.6));

		textureCat.resized(80).draw(700, 500);

		s3d::Circle(s3d::Cursor::Pos(), 60).draw(s3d::ColorF(1, 0, 0, 0.5));
	}
}
