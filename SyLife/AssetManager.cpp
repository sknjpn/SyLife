#include "Asset.h"
#include "AssetManager.h"

#include "CellAsset.h"

unique_ptr<AssetManager>	g_assetManagerPtr;

void AssetManager::init()
{

	Array<string> filepaths;

	recursive_directory_iterator end;
	for (recursive_directory_iterator it(boost::filesystem::path("assets/models")); it != end; ++it)
		if (!is_directory(*it)) filepaths.emplace_back((*it).path().string());

	for (const auto& filepath : filepaths)
	{
		ptree pt;

		read_json(filepath, pt);

		auto a = makeAsset(pt.get<string>("type"));

		a->setName(pt.get<string>("name"));
		a->setFilepath(filepath);
	}

	for (const auto& m : m_assets)
	{
		ptree pt;

		read_json(m->getFilepath(), pt);

		try
		{
			m->load(pt);
		}
		catch (boost::property_tree::ptree_bad_path & e)
		{
			LOG_ERROR(U"JSONアセットの読み込みに問題が発生しました");
			LOG_ERROR(U" What:" + Unicode::Widen(string(e.what())));
			LOG_ERROR(U" Asset:" + Unicode::Widen(pt.get<string>("type")));
			LOG_ERROR(U" Filepath:" + Unicode::Widen(m->getFilepath()));

			System::Exit();
		}
		catch (Error & e)
		{
			LOG_ERROR(U" What:" + e.what());
			LOG_ERROR(U" Asset:" + Unicode::Widen(pt.get<string>("type")));
			LOG_ERROR(U" Filepath:" + Unicode::Widen(m->getFilepath()));

			System::Exit();
		}
	}

	// CellAssetの初期化
	auto cellAssets = getAssets<CellAsset>();
	for (const auto& ca : cellAssets) ca->updateProperties();
}

shared_ptr<Asset> AssetManager::getAsset(const string& name) const
{
	for (auto it = m_assets.begin(); it != m_assets.end(); ++it)
		if ((*it)->getName() == name) return dynamic_pointer_cast<Asset>(*it);

	throw Error(U"存在しない名前のモデルを参照しました");
}
