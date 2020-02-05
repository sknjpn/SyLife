#include "PartAsset_Body.h"
#include "PartState_Body.h"

std::shared_ptr<PartState> PartAsset_Body::makePartState(const std::shared_ptr<PartConfig>& partConfig) const
{
	return MakeShared<PartState_Body>(partConfig);
}

void PartAsset_Body::load(const JSONValue& json)
{
	PartAsset::load(json);

	m_image = Image(FileSystem::ParentPath(getFilePath()) + FileSystem::BaseName(getFilePath()) + U".png");

	m_texture = Texture(m_image);
}

void PartAsset_Body::save(JSONWriter& json) const
{
	PartAsset::save(json);

	if (getIsUserAsset())
		m_image.save(U"world/assets/" + FileSystem::BaseName(getFilePath()) + U".png");
}

void PartAsset_Body::draw(double a)
{
	m_texture
		.scaled(1.0 / GeneralSetting::GetInstance().m_textureScale)
		.drawAt(Vec2::Zero(), ColorF(1.0, a));
}

void PartAsset_Body::preRender(Image& image, double scale, const std::shared_ptr<PartConfig>& partConfig, double a) const
{
	m_image.overwrite(image, Point(0, 0), ColorF(1.0, a));
}
