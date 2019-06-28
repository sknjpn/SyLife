#include <iostream>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;


int process()
{
	asio::io_service io_service;

	tcp::acceptor acc(io_service, tcp::endpoint(tcp::v4(), 31400));
	tcp::socket socket(io_service);

	// 接続待機
	acc.accept(socket);

	// メッセージ受信
	asio::streambuf receive_buffer;
	boost::system::error_code error;
	asio::read(socket, receive_buffer, asio::transfer_all(), error);

	if (error && error != asio::error::eof) {
		std::cout << "receive failed: " << error.message() << std::endl;
	}
	else {
		const char* data = asio::buffer_cast<const char*>(receive_buffer.data());
		std::cout << data << std::endl;
	}
}

void Main()
{
	process();

	Graphics::SetBackground(ColorF(0.8, 0.9, 1.0));

	const Font font(60);

	const Texture textureCat(Emoji(U"🐈"), TextureDesc::Mipped);

	while (System::Update())
	{
		font(U"Hello, Siv3D!🐣").drawAt(Window::Center(), Palette::Black);

		font(Cursor::Pos()).draw(20, 500, ColorF(0.6));

		textureCat.resized(80).draw(700, 500);

		Circle(Cursor::Pos(), 60).draw(ColorF(1, 0, 0, 0.5));
	}
}