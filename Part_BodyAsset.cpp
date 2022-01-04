﻿#include "Part_BodyAsset.h"
#include "Part_BodyState.h"
#include "PartConfig.h"

std::shared_ptr<PartState> Part_BodyAsset::makePartState(const std::shared_ptr<PartConfig>& partConfig) const
{
	return MakeShared<Part_BodyState>(partConfig);
}

void Part_BodyAsset::load(const JSONValue& json)
{
	PartAsset::load(json);

	m_image = Image(FileSystem::ParentPath(getFilePath()) + FileSystem::BaseName(getFilePath()) + U".png");

	m_texture = Texture(m_image);
}

void Part_BodyAsset::save(JSONWriter& json) const
{
	PartAsset::save(json);

	if (getIsUserAsset())
		m_image.save(U"world/assets/" + FileSystem::BaseName(getFilePath()) + U".png");
}

void Part_BodyAsset::draw(double a)
{
	m_texture
		.scaled(1.0 / GeneralSetting::GetInstance().m_textureScale)
		.drawAt(Vec2::Zero(), ColorF(1.0, a));
}

void Part_BodyAsset::preRender(Image& image, const std::shared_ptr<PartConfig>& partConfig) const
{
	m_image.overwrite(image, (partConfig->getPosition() * GeneralSetting::GetInstance().m_textureScale).asPoint() + image.size() / 2 - m_image.size() / 2, ColorF(1.0, 1.0));
}